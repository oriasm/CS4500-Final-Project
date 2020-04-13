// language: C++
#pragma once
#include <unistd.h>
#include <stdio.h>
#include "../utilities/object.h"
#include "../utilities/array.h"
#include "peer.h"

/**
 * Author: Maria Orias
 * A specialized Array of Objects that holds Peers
 */
class PeerArray : public Object
{
public:
    size_t size_;         /* Maximum size of an individual nested array */
    size_t total_length_; /* Current total length of the Array */
    size_t data_size_;    /* Maximum size of the array of Object pointers */
    size_t data_length_;  /* Current length of the current nested array */
    Peer ***data_;        /* Actual data stored within the Array, an array of Object pointers */

    /* Constructs an empty Array, initially can hold 1024 elements */
    PeerArray()
    {
        size_ = MAX_ARRAY_SIZE;
        total_length_ = 0;
        data_size_ = STARTING_DATA_ARRAY_SIZE;
        data_length_ = 0;
        data_ = new Peer **[data_size_];
    }

    /* Removes an Array from memory */
    virtual ~PeerArray()
    {
        // Remove all nested arrays
        for (size_t i = 0; i < data_length_; i++)
        {
            delete[] data_[i];
        }
        // Remove main array
        delete[] data_;
    }

    /* Returns the number of elements in an Array */
    virtual size_t length()
    {
        return total_length_;
    }

    /* Grows the Array if needed */
    virtual void grow_(size_t new_length)
    {
        // There is still room in our current array
        if (new_length < size_)
        {
            return;
        }
        // There is still room in our main data store
        if (data_length_ + 1 < data_size_)
        {
            return;
        }

        // Copy all array pointers to new array
        data_size_ = data_size_ * 2;
        Peer ***new_data = new Peer **[data_size_];
        for (size_t i = 0; i < data_length_; i++)
        {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
    }

    /* Adds an Object to the end of the Array */
    void push(Peer *o)
    {
        size_t insert_idx = total_length_ % size_;
        grow_(insert_idx + 1);
        if (insert_idx == 0)
        {
            data_[data_length_] = new Peer *[size_];
            data_length_++;
        }
        Peer **current_arr = data_[data_length_ - 1];
        current_arr[insert_idx] = o;
        total_length_++;
    }

    /* Returns an Object at a given index */
    Peer *get(size_t idx)
    {
        size_t nested_idx = idx % size_;
        size_t data_idx = idx / size_;
        Peer **current_arr = data_[data_idx];
        return current_arr[nested_idx];
    }

    /**
     * Adds an Object at a given index
     * Returns overwritten Object */
    Peer *set(size_t idx, Peer *o)
    {
        size_t nested_idx = idx % size_;
        size_t data_idx = idx / size_;
        Peer **current_arr = data_[data_idx];
        Peer *overwritten = current_arr[nested_idx];
        current_arr[nested_idx] = o;
        return overwritten;
    }

    /* Checks if this Array is equal to a given Object */
    virtual bool equals(Peer *o)
    {
        Array *casted_arr = dynamic_cast<Array *>(o);
        if (casted_arr)
        {
            return casted_arr->hash() == hash();
        }

        return false;
    }

    /* Generates a hash value for this Array */
    virtual size_t hash()
    {
        size_t hash = 0;
        size_t len_counter = 0;
        for (size_t i = 0; i < data_length_; i++)
        {
            for (size_t j = 0; j < size_; j++)
            {
                if (len_counter >= total_length_)
                {
                    break;
                }
                hash = hash * 31 + data_[i][j]->hash();
                len_counter++;
            }
        }
        return hash;
    }
};