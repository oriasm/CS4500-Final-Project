//lang::CwC
#pragma once

#include "object.h"
#include <cstddef>

// LinkedList for Objects
// This List does NOT own its elements, they must be freed independently
class LinkedList : public Object
{
public:
    Object *first_;
    LinkedList *rest_;

    // Default Constructor
    LinkedList()
    {
        first_ = NULL;
        rest_ = NULL;
    }

    ~LinkedList()
    {
        if (rest_)
            delete rest_;
    }

    // Returns the hash of this object
    virtual size_t hash()
    {
        if (first_ == NULL)
        {
            return 0;
        }
        else
        {
            size_t my_hash = first_->hash() * 31;

            if (rest_ != NULL)
            {
                my_hash += rest_->hash();
            }

            return my_hash;
        }
    }

    virtual bool equals(Object *other)
    {
        LinkedList *list = dynamic_cast<LinkedList *>(other);

        if (list)
        {
            if (size() == 0)
                return list->size() == 0;

            if (first_->equals(list->first_))
            {
                if (rest_ != NULL)
                {
                    return rest_->equals(list->rest_);
                }
                return rest_ == list->rest_;
            }

            return false;
        }

        return false;
    }

    // Returns the number of elements in the collection
    virtual size_t size()
    {
        if (first_ == NULL)
            return 0;
        if (rest_ == NULL)
            return 1;
        return 1 + rest_->size();
    }

    virtual Object *get(size_t index)
    {
        if (index >= size())
            return NULL;

        if (index == 0)
        {
            return first_;
        }
        else
        {
            return rest_->get(index - 1);
        }
    }

    /*
	 * Returns the index of the first occurrence of o,
	 * or >size() if not there
	 */
    virtual size_t index_of(Object *o)
    {
        if (first_ != NULL)
        {
            if (first_->equals(o))
            {
                return 0;
            }
            else
            {
                return rest_->index_of(o) + 1;
            }
        }

        return size() + 1;
    }

    /*
	 * Appends e to the end
	 */
    virtual void push_back(Object *e)
    {
        if (first_ == NULL)
        {
            first_ = e;
            rest_ = new LinkedList();
        }
        else
        {
            rest_->push_back(e);
        }
    }

    /*
	 * Replaces the element at i with e
	 * returns the original String a i
	 */
    virtual Object *set(size_t i, Object *e)
    {
        if (i >= size())
            return NULL;

        if (i == 0)
        {
            Object *temp = first_;
            first_ = e;
            return temp;
        }
        else
        {
            return rest_->set(i - 1, e);
        }
    }

    /*
	 * Removes the element at i
	 */
    virtual Object *remove(size_t i)
    {
        if (size() == 0)
        {
            return NULL;
        }

        if (i == 0)
        {
            Object *temp = first_;
            if (rest_ == NULL)
            {
                first_ = NULL;
            }
            else
            {
                first_ = rest_->first_;
                rest_ = rest_->rest_;
            }

            return temp;
        }

        return rest_->remove(i - 1);
    }

    /*
	 * Removes the first element
	 */
    virtual Object *remove_first()
    {
        return remove(0);
    }

    /*
	 * Removes all of elements from this list
	 */
    void clear()
    {
        first_ = NULL;
		if (rest_)
			delete rest_;
        rest_ = NULL;
    }
};