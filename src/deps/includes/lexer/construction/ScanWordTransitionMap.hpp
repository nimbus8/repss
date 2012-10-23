#ifndef _SCAN_WORD_TRANSITION_MAP_HPP_
#define _SCAN_WORD_TRANSITION_MAP_HPP_

#include <unordered_map>

#define SCAN_WORD_TRANSITION_MAP_number_of_distinct_symbols 257

class TransitionInputKey
{
private:
    unsigned int _scanWordId;

    bool _isRanged;
    bool _isAnythingBut;
    char _inputCharacter;

    bool _isDefn;
public:
    TransitionInputKey(unsigned int scanWordId, char inputCharacter, bool isRanged, bool isAnythingBut, bool isDefn) : _scanWordId(scanWordId), _isRanged(isRanged), _isAnythingBut(isAnythingBut), _inputCharacter(inputCharacter), _isDefn(isDefn) {}
    TransitionInputKey(unsigned int scanWordId, char inputCharacter, bool isRanged, bool isAnythingBut) 
       : _scanWordId(scanWordId), _isRanged(isRanged), _isAnythingBut(isAnythingBut), _inputCharacter(inputCharacter), _isDefn(false){}
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

    bool isDefn() const
    {
        return _isDefn;
    }
};

class ScanWordNode;

class ScanWordTransitionMap
{
private:
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

                //we index the defn differently, as the defn will ALWAYS come with a limited alphabet
                //  when we run on the fly, we get the full alphabet and that needs to be sorted into one of the entries in the limited alphabet
                if (transInputKey.isDefn())
                {
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
                        hashIndex =(size_t) 1 + (size_t)scanWordId*_SPACE_ALOTTED_TO_EACH_SCANWORD + (size_t)index;
                        std::cout << "\t\tDefn: Defined - Ranged ScanWord-Hash: " << hashIndex << std::endl;
                    }
                    else
                    {
                        std::cout << "Not So Serious Afterall (see what happens): could not find index mapping.::ScanWordTransitionMap" << std::endl;
                        //hashIndex = 0; //we'll guves this to mismatch
                        std::cout << "\t\tDefn: Failed to define == 7 - case - ScanWord-Hash: " << hashIndex << std::endl;

                       //fallback to normal
                hashIndex = (size_t)1 + ((size_t)scanWordId)*_SPACE_ALOTTED_TO_EACH_SCANWORD + (isAnythingBut? _SIZE_OF_RANGED_ALPHABET + _SIZE_OF_NORMAL_ALPHABET : _SIZE_OF_RANGED_ALPHABET) + (size_t)inputCharacter;

                std::cout << "\t\tNormal ScanWord-Hash: " << hashIndex << std::endl;
                    }
                }
                else
                {
                    if (islower(inputCharacter))
                    {
                        index = 1;
                    }
                    else if (isupper(inputCharacter))
                    {
                        index = 2;
                    }
                    else if (isalpha(inputCharacter))
                    {
                        index = 3;
                    }
                    else if (inputCharacter == '0')
                    {
                        index = 4;
                    }
                    else if (inputCharacter >= '1' && inputCharacter <= '9')
                    {
                        index = 5;
                    }
                    else if (inputCharacter >= '0' && 'inputCharacter <= 9')
                    {
                        index = 6;
                    }
                    else
                    {
                        index = 0;
                    }

                    hashIndex = (size_t)1 + (size_t)scanWordId*_SPACE_ALOTTED_TO_EACH_SCANWORD + (size_t)index;
                    std::cout << "\t\tRanged ScanWord-Hash: " << hashIndex << std::endl;
                }
            }
            else
            {
                //this looks about right
                hashIndex = (size_t)1 + ((size_t)scanWordId)*_SPACE_ALOTTED_TO_EACH_SCANWORD + (isAnythingBut? _SIZE_OF_RANGED_ALPHABET + _SIZE_OF_NORMAL_ALPHABET : _SIZE_OF_RANGED_ALPHABET) + (size_t)inputCharacter;

                std::cout << "\t\tNormal ScanWord-Hash: " << hashIndex << std::endl;
           }

            return hashIndex;
        }
    };

    class TransitionInputKeyEquals {
    public:
        bool operator ()(const TransitionInputKey &lhs, const TransitionInputKey &rhs) const
        {
          TransitionInputKeyHashFunction hashFunc;

          return hashFunc(lhs) == hashFunc(rhs);

        }
    };

    std::unordered_map<TransitionInputKey, ScanWordNode*, TransitionInputKeyHashFunction, TransitionInputKeyEquals> _nextScanWordNode;

public:

    ScanWordTransitionMap() {}
    ~ScanWordTransitionMap() {}

    void emplace(std::pair<TransitionInputKey, ScanWordNode*>& keyAndValue)
    {
        _nextScanWordNode.emplace(keyAndValue);

        std::cout << "ScanWordTransitionMap::emplace(...) called. Size now: " << _nextScanWordNode.size() << std::endl;
        std::cout << "\t { " << keyAndValue.first.getScanWordId() << ", " << keyAndValue.first.getIsRanged() << ","
                  << keyAndValue.first.getIsAnythingBut() << ", " << keyAndValue.first.getInputCharacter()
                  << " } -> Scan-Word()" << std::endl;
    }

    ScanWordNode* getNextScanWordNode(const TransitionInputKey &key) const
    {
        std::cout << "\t ::ScanWordTransitionMap with input { " << key.getScanWordId() << ", " << key.getIsRanged() << ","
                  << key.getIsAnythingBut() << ", " << key.getInputCharacter() << " }" << std::endl;

        ScanWordNode* ret;

        auto fetched = _nextScanWordNode.find(key);

        if (fetched != _nextScanWordNode.end())
        {
            ret = fetched->second;

            std::cout << "FOUND a match!" << std::endl;
        }
        else
        {
            ret = nullptr;
            std::cout << "Could NOT find match" << std::endl;
        }

       return ret;
    }

    friend class ScanWordNode;
};

#endif
