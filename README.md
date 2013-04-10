/[(r | e | p | s | s)]/

This project is written in "C++11", for no particular reason.

What is repss?

  Repss is idea related to context free grammars and the potential infinitum of languages; where such intresting tidbits
  exist that say things like 'many valid languages cannot be recognized' given "machines" wholly deterministic in nature
  (or something like that, don't quote me (unless on an exam, then go right ahead)). This is the kind of stuff that fascinates me,
  to the point where I wouldn't mind teaching theory of computation to undergrads. But of course, I'm probably never going
  to get a PhD, nor should anyone give me one for that matter; thus it's unlikely I'll ever teach undergrads (thank god...),
  or theory of computation (to anyone who will listen). This project has two facets: 

    1) The first facet is for those who are interested in understanding compilers, and a bit of what they take to build
    2) The second facet is for most people, so we'll see too it first
    

  2)
  Repss is in part designed to be **a scaffolding on top of written language**. The designer of the language stresses
  that the whole concept is inherently evil and mad-making, and one should seriously think about the potential ramification
  or damage to their proper functioning before they invest any amount time into learning something so...ultimately, very stupid.
  But still he believes that the language could be an incredably beneficial tool (a boon) for writers *[1]*, should they choose to use it;
  and to elementary school teachers, who might leverage it to enhance their curriculums and promote greater creativity, pride, and ownership
  in student writing. It should be noted that the designer has done no practical work to validate such a hypothesis, and quite
  honestly doesn't /[(care | intend)>>or<] to, ever. But a quick perusal of the usage-01.txt file, under the bin/ directory at
  the root, should be enough to confirm whether or not this project, or the language at the center of it, might be of any utility or
  at all relevant to the inquisitive individual -- though be warned: should one read it, they should mind the profanity (where it occurs)
  and instead focus on how they might potentially leverage the scaffolding to their benefit (as a literary device: to enhance their own creativity;
  to manage their fractured and non-linear thinking patterns; to better represent, explore, and refine their ideas as they come, etc), and
  should they find it useful, interesting, or worthy of a book deal, they might consider emailing me, or *sending me money* if they'd 
  like for the project to be completed much sooner than it would otherwise aided by their valued input and perhaps a few words of encouragement. Just an idea.

  
  1)
  The other part of repss is the code. A look at the code should give one insight on the sort of decisions that
  must be made along the way. I don't like being held back by languages, so there are very feww limitations here
  to speak of (at least not in the specification)
  Hopefully comments are helpful, and my code clear, as I've made a conscious effort to write for others, and not
  just myself. So, in theory, other programmers, with an appropriately thourough background in basic data structures
  (stacks, maps, trees, etc), should understand it . It should be enough to understand what considerations there are to
  made that pop up when trying to tackle fundamental problems. Seeing as how there are usually many ways to
  do something, I almost always note why I made the decision to go any one way - unless it were trivially obvious why (and I didn't care to elaborate);
  The code, was designed on some levels with speed in mind, but only after "flexibility" and "expresiveness" have been
  establshed. The overall concept (lexing+analysis+generation) is fairly complex - without my help - so a well-designed
  backend architecture was necessary, if it isn't the most important thing, period [note 1]. As long as the principle ideas
  are sound, I like to think if you have a well well defined structure from the start, most if not everything will just fall
  into place (it may just have went to hell if it weren't). 
  
  Some notes on the Technical Specifications
  ------------------------------------------
  The organization of the project was architected to be as simple as possible; split into seperate independant modules,
  while keeping things consistent, and available only when and where their needed. I should add that the way we accomplish
  this is through a sub-project whose source is entirely indepedant from the main project. A common theme here (if you catch it,the drift...)
  is 'meta'. I don't like to consider Repss as a meta-language, nor do I want to consider repss as a translator; both are arguably just that
  but I believe my argument to the contrary holds more weight, at least here in this tiny locality of textual-processing. My point here is to
  convince you that compilers are important, and the world didn't end with lisp, as some are so adament - though I do love lisp.
  I actually learnt lisp AFTER I finished much of the project, ironically. Picking it up was easy, because I dont think it's true
  to say once youve learnt one language youve learnt them all, but more true to say once you've created from top to bottom a compiler
  for one language, you've pretty much done them all. I 'm not sure, but I think here I do it twice, if not thrice...
  





Footnotes:

[1] *maybe just the mediocre writers like myself...b/c I hear most good writers, the ones worth reading, can write an entire
   novel over the course of a weekend; just one continuous draft written in ink, preferably by feather, without even a hint
   of inspiration - because they have incredably long fingers or a sort of visible abnormality(disfigurement) that makes 
   them different from most people. They're monsters/[(**.|;/[(|**they're)] mutants, /[(/[(|quite)] possibly /[(|**a)] 
   | /[(|**maybe)] even /[(|**a)] | we should count them /[(as | a)>><])] diferent species!)]  I don't know about you, but I'm f***ing *jealous!*




