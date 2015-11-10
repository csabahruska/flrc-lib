/*
 * COPYRIGHT_NOTICE_1
 */

#ifndef _LARGE_BITMASK_
#define _LARGE_BITMASK_

#include <stdexcept>

class VarBitmask {
protected:
    unsigned *m_data;
    unsigned  m_bit_length;
    unsigned  m_dword_length;
    VarBitmask(unsigned *data,unsigned bit_len,unsigned word_len) :
        m_data(data),
        m_bit_length(bit_len),
        m_dword_length(word_len) {}
public:
    VarBitmask(void) :
        m_data(NULL),
        m_bit_length(0),
        m_dword_length(0) {}

   ~VarBitmask(void) {
        delete [] m_data; // find is m_data is NULL
    }

    void set_bit_length(unsigned bit_length) {
        assert(!m_data); // for now assume that we won't be changing the size
        assert(bit_length > 0);
        m_bit_length   = bit_length;
        m_dword_length = ((m_bit_length + 31) / 32);
        assert(m_dword_length > 0);
        m_data = new unsigned[m_dword_length];
        memset(m_data,0,sizeof(unsigned) * m_dword_length);
    }

    VarBitmask set_union(const VarBitmask &other) const {
        if(m_bit_length != other.m_bit_length) {
            assert(0);
            throw std::length_error("set_union in VarBitmask not equal length");
        } else {
            unsigned *new_data = new unsigned[m_dword_length];
            unsigned i;
            for(i=0; i<m_dword_length; ++i) {
                new_data[i] = m_data[i] | other.m_data[i];
            }
            return VarBitmask(new_data,m_bit_length,m_dword_length);
        }
    }

    VarBitmask set_intersection(const VarBitmask &other) const {
        if(m_bit_length != other.m_bit_length) {
            assert(0);
            throw std::length_error("set_intersection in VarBitmask not equal length");
        } else {
            unsigned *new_data = new unsigned[m_dword_length];
            unsigned i;
            for(i=0; i<m_dword_length; ++i) {
                new_data[i] = m_data[i] & other.m_data[i];
            }
            return VarBitmask(new_data,m_bit_length,m_dword_length);
        }
    }

    VarBitmask & operator=(const VarBitmask &other) {
        if(m_bit_length != other.m_bit_length) {
            assert(0);
            throw std::length_error("operator= in VarBitmask not equal length");
        } else {
            unsigned i;
            for(i=0; i< m_dword_length; ++i) {
                m_data[i] = other.m_data[i];
            }
            return *this;
        }
    }

    bool operator!=(const VarBitmask &other) {
        if(m_bit_length != other.m_bit_length) {
            assert(0);
            throw std::length_error("operator!= in VarBitmask not equal length");
        } else {
            unsigned i;
            for(i=0; i< m_dword_length; ++i) {
                if(m_data[i] != other.m_data[i]) return true;
            }
            return false;
        }
    }

    void self_union(const VarBitmask &other) const {
        if(m_bit_length != other.m_bit_length) {
            assert(0);
            throw std::length_error("self_union in VarBitmask not equal length");
        } else {
            unsigned i;
            for(i=0; i<m_dword_length; ++i) {
                m_data[i] |= other.m_data[i];
            }
        }
    }

    void self_intersection(const VarBitmask &other) const {
        if(m_bit_length != other.m_bit_length) {
            assert(0);
            throw std::length_error("self_intersection in VarBitmask not equal length");
        } else {
            unsigned i;
            for(i=0; i<m_dword_length; ++i) {
                m_data[i] &= other.m_data[i];
            }
        }
    }

    bool get(unsigned index) {
        assert(index < m_bit_length);
        unsigned dword_index = index / 32;
        unsigned bit_index   = index % 32;
        if(m_data[dword_index] & (1 << bit_index)) {
            return true;
        } else {
            return false;
        }
    }

    void set(unsigned index) {
        assert(index < m_bit_length);
        unsigned dword_index = index / 32;
        unsigned bit_index   = index % 32;
        m_data[dword_index] |= (1 << bit_index);
    }

    void unset(unsigned index) {
        assert(index < m_bit_length);
        unsigned dword_index = index / 32;
        unsigned bit_index   = index % 32;
        m_data[dword_index] &= ~(1 << bit_index);
    }

    void set_all(void) {
        unsigned i;
        for(i=0; i < m_dword_length-1; ++i) {
            m_data[i] = 0xFFffFFff;
        }
        unsigned shift_amount = (32 - (m_bit_length % 32));
        if(shift_amount != 32) {
            m_data[i] = (0xFFffFFff >> shift_amount);
        }
    }
};

#endif // _LARGE_BITMASK_

