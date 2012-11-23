/*
 REPSS
 Copyright (C) 2012  Khalique Williams

 This file is part of REPSS.

 REPSS is free software: you can redistribute it and/or modify
 it under the terms of the   GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 REPSS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with REPSS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lib/includes/execution_phase/grammaticalForm/GrammarConfig.hpp"
#include "lib/includes/execution_phase/grammaticalForm/GrammarKeywordDefn.hpp"

#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif


GrammarConfig::GrammarConfig()
{
    //GrammarKeywords are already as simply Keywords.
    // - Each keyword has name and grammar type

    //So here, what we do is define MORE SPECIFICALLY each type
    // todo:will So we need to add to keyword generator so that FOR EACH
    //            keyword, we force language writer to specfy terms for
    //            for EACH keyword. So for instance we might define that yes
    //            reps.named_iteration had a grammarType VARIABLE,
    //            but exactly what can it expand to?
    //                RepsNamedItertation -> ANY  general_end
    //                 ANY -> RepsNamedIteration general_end |
    //                        scope general_end |
    //                        alteration |
    //                        evaluation
    //
    // NOTE: This can ALL be done through the keywordGenerator. For now NO.
    //        Perhaps ALL would be best, but I need to work fast. This is
    //        faster (? maybe - if it gets to be to much bite the bullet),
    //        and seems more concret as it's evident that we're requesting
    //        to define a particular keyword...instead of assuming one
    //        we defined later, and having to build up an internal repr.
    //        inside keyword generator that checks itself and w/e.
    //        I'd like to keep keyword generator straightforward (for now...)
    //
    // The way the code should look:
    //
    // class GrammarConfig : public AbstrGrammarConfig
    // {
    //     Keywords* _keywords;
    //     ...
    //     void _defineGrammarKeyword_RepititionNamedIteration(std::pair<std::string, std::string> nameAndGrammarType)
    //     {
    //    
    //     }
    // }
    //
    //  To make things easier for generator we'll modify Keywords to have
    //   public function that acts like a map (retrieves GrammarType for a keyword name (easy, b/c in generator we know what index belongs to what object)
    //    or returns an INVALID or UNDEFINED. So, we can generate boiler
    //    plate for AbstrGrammarConfig that queries Keywords class using
    //    name(pretty much the key(main identifier) for every keyword in 
    //    generator). And we can serve a new distinct object to each
    //    parameter of defineGrammar....() functions and forcing it to return
    //    a class of some sort with all the grammar expansion rules. The
    //    combined data (name, type, (grammar-expansion-rules)) will make up
    //    new data object to be used in execution (instead of KeywordsData
    //    in Keywords).
    //
    // The way GENERATED CODE should look like:
    // class AbstrGrammarConfig
    // {
    // protected:
    //     ReadOnlyData* _keywords;  //change this to something more specific?
    //
    //     void _defineGrammarKeyword_RepititionNamedIteration(std::pair<std::string, std::string> nameAndGrammarType__RepititionNamedIteration__VARIABLE) = 0;
    //     void _defineGrammarKeyword_Scope(std::pair<std::string, std::string> nameAndGrammarType__Scope__VARIABLE) = 0;
    //                  . .
    //                  . .
    //     void init()
    //     {
    //         _defineGrammarKeyword_RepititionNamedIteration(std::make_pair<std::string,std::string>("reps.named_iteration", _keywords.getTypeForName("reps.named_iteration")));
    //         _defineGrammarKeyword_Scope(std::make_pair<std::string,std::string>("scope", _keywords.getTypeForName("scope")));
    //         ....
    //         ....
    //         ....
    //     }
    // }
}

#undef DEBUG
#undef DeLOG
