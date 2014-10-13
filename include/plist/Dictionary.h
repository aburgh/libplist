/*
 * Dictionary.h
 * Dictionary node type for C++ binding
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

#ifndef PLIST_DICTIONARY_H
#define PLIST_DICTIONARY_H

#include <plist/Structure.h>
#include <map>
#include <string>

namespace PList
{

class Dictionary : public Structure
{
public :
    Dictionary(Node* parent = NULL);
    Dictionary(plist_t node, Node* parent = NULL);
    Dictionary(const Dictionary& d);
    Dictionary& operator=(Dictionary& d);
    virtual ~Dictionary();

    Node* Clone() const;

	using iterator = std::map<std::string,Node*>::iterator;
	using const_iterator = std::map<std::string,Node*>::const_iterator;
	using reverse_iterator = std::map<std::string,Node*>::reverse_iterator;
	using const_reverse_iterator = std::map<std::string,Node*>::const_reverse_iterator;

    Node* operator[](const std::string& key);
    iterator Begin();
    iterator End();
    iterator Find(const std::string& key);
    iterator Set(const std::string& key, const Node* node);
    iterator Set(const std::string& key, const Node& node);
    iterator Insert(const std::string& key, Node* node) PLIST_WARN_DEPRECATED("use Set() instead");
    void Remove(Node* node);
    void Remove(const std::string& key);
    std::string GetNodeKey(Node* key);

	/* Iterators */

	iterator begin() noexcept {
		return _map.begin();
	}

	iterator end() noexcept {
		return _map.end();
	}

	const_iterator cbegin() const noexcept {
		return _map.cbegin();
	}

	const_iterator cend() const noexcept {
		return _map.cend();
	}

	reverse_iterator rbegin() noexcept {
		return _map.rbegin();
	}

	reverse_iterator rend() noexcept {
		return _map.rend();
	}

	const_reverse_iterator crbegin() const noexcept {
		return _map.crbegin();
	}

	const_reverse_iterator crend() const noexcept {
		return _map.crend();
	}

private :
    std::map<std::string,Node*> _map;


};

};

#endif // PLIST_DICTIONARY_H
