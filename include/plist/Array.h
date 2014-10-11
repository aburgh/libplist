/*
 * Array.h
 * Array node type for C++ binding
 *
 * Copyright (c) 2009 Jonathan Beck All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef PLIST_ARRAY_H
#define PLIST_ARRAY_H

#include <plist/Structure.h>
#include <vector>

namespace PList
{

class Array : public Structure
{
public :
    Array(Node* parent = NULL);
    Array(plist_t node, Node* parent = NULL);
    Array(const Array& a);
    Array& operator=(Array& a);
    virtual ~Array();

    Node* Clone() const;

    Node* operator[](unsigned int index);
    void Append(Node* node);
    void Insert(Node* node, unsigned int pos);
    void Remove(Node* node);
    void Remove(unsigned int pos);
    unsigned int GetNodeIndex(Node* node) const;

	/* Iterators */

	std::vector<Node*>::iterator begin() noexcept {
		return _array.begin();
	}

	std::vector<Node*>::iterator end() noexcept {
		return _array.end();
	}

	std::vector<Node*>::const_iterator cbegin() const noexcept {
		return _array.cbegin();
	}

	std::vector<Node*>::const_iterator cend() const noexcept {
		return _array.cend();
	}

	std::vector<Node*>::reverse_iterator rbegin() noexcept {
		return _array.rbegin();
	}

	std::vector<Node*>::reverse_iterator rend() noexcept {
		return _array.rend();
	}

	std::vector<Node*>::const_reverse_iterator crbegin() const noexcept {
		return _array.crbegin();
	}

	std::vector<Node*>::const_reverse_iterator crend() const noexcept {
		return _array.crend();
	}

private :
    std::vector<Node*> _array;
};

};

#endif // PLIST_ARRAY_H
