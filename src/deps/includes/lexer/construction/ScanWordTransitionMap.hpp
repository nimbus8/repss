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
        TransitionInputKeyHashFunction() {}

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
                        //std::cout << "\t\tDefn: Defined - Ranged ScanWord-Hash: " << hashIndex << std::endl;
                    }
                    else
                    {
                        //std::cout << "Not So Serious Afterall (see what happens): could not find index mapping.::ScanWordTransitionMap" << std::endl;
                        //hashIndex = 0; //we'll guves this to mismatch
                        //std::cout << "\t\tDefn: Failed to define == 7 - case - ScanWord-Hash: " << hashIndex << std::endl;

                       //fallback to normal
                hashIndex = (size_t)1 + ((size_t)scanWordId)*_SPACE_ALOTTED_TO_EACH_SCANWORD + (isAnythingBut? _SIZE_OF_RANGED_ALPHABET + _SIZE_OF_NORMAL_ALPHABET : _SIZE_OF_RANGED_ALPHABET) + (size_t)inputCharacter;

                //std::cout << "\t\tNormal ScanWord-Hash: " << hashIndex << std::endl;
                    }
                }
                else
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
                            index = 0; //should never happen, but if so lets have it blow up
                            break;
                    };

                    hashIndex = (size_t)1 + (size_t)scanWordId*_SPACE_ALOTTED_TO_EACH_SCANWORD + (size_t)index;
                    //std::cout << "\t\tRanged ScanWord-Hash: " << hashIndex << std::endl;
                }
            }
            else
            {
                //this looks about right
                hashIndex = (size_t)1 + ((size_t)scanWordId)*_SPACE_ALOTTED_TO_EACH_SCANWORD + (isAnythingBut? _SIZE_OF_RANGED_ALPHABET + _SIZE_OF_NORMAL_ALPHABET : _SIZE_OF_RANGED_ALPHABET) + (size_t)inputCharacter;

                if (isAnythingBut)
                    std::cout << "\t\tAnything but ScanWord-Hash: " << hashIndex << std::endl;
                else
                    std::cout << "\t\tNormal ScanWord-Hash: " << hashIndex << std::endl;

           }

            return hashIndex;
        }
    };

    class TransitionInputKeyEquals {
    public:
        bool operator ()(const TransitionInputKey &lhs, const TransitionInputKey &rhs) const
        {
          const TransitionInputKeyHashFunction hashFunc;

          return hashFunc(lhs) == hashFunc(rhs);

        }
    };

    std::unordered_map<TransitionInputKey, ScanWordNode*, TransitionInputKeyHashFunction, TransitionInputKeyEquals> _nextScanWordNode;

public:

    ScanWordTransitionMap() {}
    ~ScanWordTransitionMap() {}

    void emplace(const std::pair<TransitionInputKey, ScanWordNode*>& keyAndValue)
    {

       if (keyAndValue.first.getIsAnythingBut())
       {
           for (int c = 0; c < 255; c++)
           {
               if (((char)c) != keyAndValue.first.getInputCharacter())
               {
                   TransitionInputKey anythingButKey(keyAndValue.first.getScanWordId(), (char) c, keyAndValue.first.getIsRanged(), true, true);
                   std::cout << "{" << anythingButKey.getScanWordId() << ", " << anythingButKey.getIsRanged() << ", " << anythingButKey.getIsAnythingBut() << "}" << anythingButKey.getInputCharacter() << std::endl;
                   _nextScanWordNode.emplace(anythingButKey);
               }
           }

           std::cout << "Added entire alphabet not " << keyAndValue.first.getInputCharacter() << " to table for scanword(" << keyAndValue.first.getScanWordId() << ")" << std::endl;
       }
       else
       {
          _nextScanWordNode.emplace(keyAndValue);
       
          std::cout << "ScanWordTransitionMap::emplace(...) called. Size now: " << _nextScanWordNode.size() << std::endl;
          std::cout << "\t { " << keyAndValue.first.getScanWordId() << ", " << keyAndValue.first.getIsRanged() << ","
                  << keyAndValue.first.getIsAnythingBut() << ", " << keyAndValue.first.getInputCharacter()
                  << " } -> Scan-Word()" << std::endl;
       }
    }


    //We may want to split transition key into TransitionDefnKey and TransitionAccessKey as a further abstraction, we'l convert TransitionDefn key to TransitionAccessKey to set table. Also, access key...we may want to load more info on it, even if said info wont be used as ordinal in hashfunc. idunno, maybe nonsense, think about it - maybe through function overloading(or not) take in RangedTransactionAccessKey and UnrangedTransactionAccessKey, it seems like this func below balloons.
    const ScanWordNode* getNextScanWordNode(const TransitionInputKey &key) const
    {
       // std::cout << "\t ::ScanWordTransitionMap::getNextScanWordNode with input { " << key.getScanWordId() << ", " << key.getIsRanged() << ","
       //           << key.getIsAnythingBut() << ", " << key.getInputCharacter() << " }" << std::endl;

        const ScanWordNode* ret = nullptr;

        //recent: for ranged in particular we have to handle the sub cases, things are only defined specifcally as [0-9] or [1-9], 1 being in both for eg.

        const auto isKeyRanged = key.getIsRanged();

        if (isKeyRanged)
        {
            const auto scanWordId = key.getScanWordId();
            const auto inputCharacter = key.getInputCharacter();
            const auto isKeyAnythingBut = key.getIsAnythingBut();

                    if (isalpha(inputCharacter))
                    {
                        if (islower(inputCharacter))
                        {
                            const TransitionInputKey lowerCaseKey(scanWordId, SI_CHARS_LOWER, isKeyRanged, isKeyAnythingBut);
                            auto fetched = _nextScanWordNode.find(lowerCaseKey);

                            if (fetched != _nextScanWordNode.end())
                            {
                                ret = fetched->second;

                                //std::cout << "FOUND a match!" << std::endl;
                            }
                        }
                        else if (isupper(inputCharacter))
                        {
                            const TransitionInputKey upperCaseKey(scanWordId, SI_CHARS_UPPER, isKeyRanged, isKeyAnythingBut);
                            auto fetched = _nextScanWordNode.find(upperCaseKey);

                            if (fetched != _nextScanWordNode.end())
                            {
                                ret = fetched->second;

                                //std::cout << "FOUND a match!" << std::endl;
                            }
                        }
         
                        //put this first???? prob right?
                        if (ret == nullptr)
                        {
                            const TransitionInputKey lowerCaseKey(scanWordId, SI_CHARS_ANY, isKeyRanged, isKeyAnythingBut);
                            auto fetched = _nextScanWordNode.find(key);

                            if (fetched != _nextScanWordNode.end())
                            { 
                                ret = fetched->second;

                                //std::cout << "FOUND a match!" << std::endl;
                            }
                        }
                    }
                    else if (inputCharacter == '0')
                    {
                            const TransitionInputKey zeroKey(scanWordId, SI_NUMBERS_0, isKeyRanged, isKeyAnythingBut);
                            auto fetched = _nextScanWordNode.find(zeroKey);

                            if (fetched != _nextScanWordNode.end())
                            {
                                ret = fetched->second;

                                //std::cout << "FOUND a match!" << std::endl;
                            }
                            else
                            {
                                const TransitionInputKey allDigitsKey(scanWordId, SI_NUMBERS_0to9, isKeyRanged, isKeyAnythingBut);
                                auto fetchedZeroToNine = _nextScanWordNode.find(allDigitsKey);

                                if (fetchedZeroToNine != _nextScanWordNode.end())
                                {
                                    ret = fetchedZeroToNine->second;
                                }
                                else
                                {
                                    ret = nullptr;
                                }
                            }
                    }
                    else if (inputCharacter >= '1' && inputCharacter <= '9')
                    {
                            const TransitionInputKey allDigitsKey(scanWordId, SI_NUMBERS_0to9, isKeyRanged, isKeyAnythingBut);
                            auto fetchedZeroToNine = _nextScanWordNode.find(allDigitsKey);

                            if (fetchedZeroToNine != _nextScanWordNode.end())
                            {
                                ret = fetchedZeroToNine->second;

                                //std::cout << "FOUND a match!" << std::endl;
                            }
                            else
                            {
                                const TransitionInputKey oneToNineKey(scanWordId, SI_NUMBERS_1to9, isKeyRanged, isKeyAnythingBut);
                                auto fetchedOneToNine = _nextScanWordNode.find(oneToNineKey);
                    
                                if (fetchedOneToNine != _nextScanWordNode.end())
                                {
                                    ret = fetchedOneToNine->second;
                                }
                                else
                                {
                                    ret = nullptr;
                                }
                            }
                    }
        }
  
     if (ret == nullptr)
       {
        
        auto fetched = _nextScanWordNode.find(key);

        if (fetched != _nextScanWordNode.end())
        {
            ret = fetched->second;

            //std::cout << "FOUND a match!" << std::endl;
        }
        else
        {
            ret = nullptr;
            std::cout << "Could NOT find match" << std::endl;
        }


        }

       return ret;
    }

    friend class ScanWordNode;
};

#endif
