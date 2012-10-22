#ifndef _SCAN_WORD_TRANSITION_MAP_HPP_
#define _SCAN_WORD_TRANSITION_MAP_HPP_

#include <unordered_map>
#include "ScanWordNode.hpp"

#define SCAN_WORD_TRANSITION_MAP_number_of_distinct_symbols 257

class TransitionInputKey
{
private:
    unsigned int _scanWordId;

    bool _isRanged;
    bool _isAnythingBut;
    char _inputCharacter;
public:
    TransitionInputKey(unsigned int scanWordId, char inputCharacter, bool isRanged, bool isAnythingBut) 
       : _scanWordId(scanWordId), _isRanged(isRanged), _isAnythingBut(isAnythingBut), _inputCharacter(inputCharacter){}
    ~TransitionInputKey() {}

    unsigned int getScanWordId() const
    {
        return _scanWordId;
    }

    bool getIsRanged() const
    {
        return _isRanged;
    }

    bool getIsAnythingBut() const
    {
        return _isAnythingBut;
    }

    char getInputCharacter() const
    {
        return _inputCharacter;
    }
};

class ScanWordTransitionMap
{
private:
    static ScanWordTransitionMap* _self;

    int count;
    char c;
protected:
    class TransitionInputKeyHashFunction {
    public:
        ::std::size_t operator ()(const TransitionInputKey &transInputKey) const
        {
            const size_t _SIZE_OF_RANGED_ALPHABET = 8;
            const size_t _SIZE_OF_NORMAL_ALPHABET = 257;
            const size_t _SIZE_OF_ANYTHING_BUT_ALPHABET = 257;

            const size_t _SPACE_ALOTTED_TO_EACH_SCANWORD = _SIZE_OF_RANGED_ALPHABET + _SIZE_OF_NORMAL_ALPHABET + _SIZE_OF_ANYTHING_BUT_ALPHABET;

            size_t hashIndex;

            const auto scanWordId = transInputKey.getScanWordId();
            const auto isRangedInput = transInputKey.getIsRanged();

            const auto isAnythingBut = transInputKey.getIsAnythingBut();
            const auto inputCharacter = transInputKey.getInputCharacter();

            if (isRangedInput)
            {
                //do the calculation to determine just which ordinal need be used
                size_t index;

                switch (inputCharacter)
                {
                    case SI_EMPTY:
                        index = 0;
                        break;
                    case SI_CHARS_LOWER:
                        index = 1;
                        break;
                    case SI_CHARS_UPPER:
                        index = 2;
                        break;
                    case SI_CHARS_ANY:
                        index = 3;
                        break;
                    case SI_NUMBERS_0:
                        index = 4;
                        break;
                    case SI_NUMBERS_1to9:
                        index = 5;
                        break;
                    case SI_NUMBERS_0to9:
                        index = 6;
                        break;
                    default:
                        index = 7; //should never happen, but if so lets have it blow up
                        break;
                };

                if (index != 7)
                {
                    hashIndex = scanWordId*_SPACE_ALOTTED_TO_EACH_SCANWORD + index;
                }
                else
                {
                    std::cout << "Serious Error: could not find index mapping.::ScanWordTransitionMap" << std::endl;
                    exit(1); 
                }
            }
            else
            {
                //this looks about right
                hashIndex = ((size_t)scanWordId)*_SPACE_ALOTTED_TO_EACH_SCANWORD + (isAnythingBut? _SIZE_OF_RANGED_ALPHABET + _SIZE_OF_NORMAL_ALPHABET : _SIZE_OF_RANGED_ALPHABET) + (size_t)inputCharacter;
           }

            return hashIndex;
        }
    };

    class TransitionInputKeyEquals {
    public:
        bool operator ()(const TransitionInputKey &lhs, const TransitionInputKey &rhs) const
        {
            return ((lhs.getIsRanged() == rhs.getIsRanged()) &&
                    (lhs.getIsAnythingBut() == rhs.getIsAnythingBut()) &&
                    (lhs.getInputCharacter() == rhs.getInputCharacter()));
        }
    };

    std::unordered_map<TransitionInputKey, ScanWordNode*, TransitionInputKeyHashFunction, TransitionInputKeyEquals> _nextScanWordNode;

public:

    ScanWordTransitionMap() : count(0),  c('a') {}

    static ScanWordTransitionMap* getInstance();

    ~ScanWordTransitionMap() {}

    void emplace(std::pair<TransitionInputKey, ScanWordNode*>& keyAndValue)
    {
        _nextScanWordNode.emplace(keyAndValue);
    }

    ScanWordNode* getNextScanWordNode(const TransitionInputKey &key) const
    {
        ScanWordNode* ret;

        auto fetched = _nextScanWordNode.find(key);

        if (fetched != _nextScanWordNode.end())
        {
            ret = fetched->second;
        }
        else
        {
            ret = nullptr;
        }
    }

    int incr() {return ++count;} 

    friend class ScanWordNode;
};

#endif
