/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_getter.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzolf <hmartzol@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by hmartzolf         #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by hmartzolf        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_GETTER_H
# define BLOCK_GETTER_H

# include <stdint.h>
# include <stddef.h>

/**
* describe a block of data to be read
* @param word_size: in bytes, the amount of data that represent a word
* @param block_size: in words, 64 bytes of 4 bytes words will result in 16
* @param big_endian: should the words be transformed to big endian
* @param func: finishing function, might return 1 to be called again
*/

typedef struct s_bg_descriptor	t_bg_descriptor;

/**
* stored data on how an input (string/file) will be read block by block
* should only be used using the constructors, never instantiated manually
* @param finished: is there no more data to be read/descriptor function to call
* @param head_or_fd: in str mode, head of reading, in file mode, file descriptor
* @param str_ptr: in str mode, pointer to the start of the string
* @param forward: if >= 0, print all that is read to this file descriptor
* @param reader: function responsible to read the file/string using this reader
* @param size: total size of the data read so far
* @param rep: amount of times the descriptor function was called
*/

typedef struct s_bg_reader		t_bg_reader;

/**
* called when reaching the end of string/file or if another call was requested
* @param buff: current buffer
* @param read: how much of the buffer is already populated
* @param total_size: total cumulative size of reads so far
* @param repeat: how many times this function was called
* @return: if != 0, will be called another time on next iteration
*/

typedef int						(*t_bg_finish)(uint8_t *buff, size_t read,
	size_t total_size, size_t repeat);

/**
* called when reading a block
* @param self: pointer to the reader holding this function
* @param buff: buffer to copy/read into
* @param descriptor: used to determine how much should be read on this iteration
*/

typedef size_t					(*t_bg_inner_read)(t_bg_reader *self,
	uint8_t *buff, const t_bg_descriptor *descriptor);

struct							s_bg_descriptor
{
	size_t		word_size;
	size_t		block_size;
	int			big_endian;
	t_bg_finish	func;
};

struct							s_bg_reader
{
	int				finished;
	int				head_or_fd;
	const char		*str_ptr;
	int				forward;
	t_bg_inner_read	reader;
	size_t			size;
	size_t			rep;
};

/**
* construct a reader that will iterate on a string
* @param str: to iterate onto
* @param forward: if >= 0, also print the content of the string to this fd
* @return: a structure ready to be used by read_block
*/

t_bg_reader						str_reader(const char *str, int forward);

/**
* construct a reader that will iterate on a file
* @param fd: file to be read
* @param forward: if >= 0, also print the content of the file to this fd
* @return: a structure ready to be used by read_block
*/

t_bg_reader						fd_reader(int fd, int forward);

/**
* read_block: function that read a single block from the given reader and apply
* some transformations according to the descriptor
* @param desc: describes how the read data will be transformed
* @param reader: current state of the reader, will be modified
* @param buff: output buffer, should be large enough to fit an entire block
* @return: 1 if there is still data to be read/generated, 0 if finished
*/

int								read_block(const t_bg_descriptor *desc,
									t_bg_reader *reader,
									void *buff);

#endif
