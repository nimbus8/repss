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
  Repss is in part designed to be **a scaffolding atop written language**. The designer of the language first stresses
  that the whole concept is inherently evil and mad-making, and one should seriously think (twice) about the potential ramification
  or damage to their proper functioning before they invest any amount time into learning something so...ultimately, very stupid.
  But still he believes that the language could be an incredably beneficial tool (a boon) for writers *[1]*, should they choose to use it;
  and to elementary school teachers, who might leverage it to enhance their curriculums and promote greater creativity, pride, and ownership
  in student writing. It should be noted also that the designer has done no practical work to validate such a hypothesis, and quite
  honestly doesn't /[(care | intend)>>or<] to, ever. But a quick perusal of the usage-01.txt file, under the bin/ directory at
  the root, should be enough to confirm whether or not this project, or the language at the center of it, might be of any utility or
  at all relevant to the inquisitive individual -- though be warned: should one read it, they should mind the profanity (where it occurs)
  and instead focus on how they might potentially leverage the scaffolding to their benefit (as a literary device: to enhance their own creativity;
  to manage their fractured and non-linear thinking patterns; and better represent, explore, and refine their ideas as they come, etc) - and
  should they find it useful, interesting, or worthy of a book deal, they might consider emailing me, or *sending me money (kwillia.pub@gmail.com)* if they'd 
  like to see this project completed much sooner than it would otherwise. I cherish the value of your input; and I'm optimistic that any of said input,
  whether it avoid or maybe to the contrary (should or it shouldnt) perhaps include a few (kind) words of encouragement, it will be just enough to help me
  forget about the more uninteresting things in my life like room and board, help me (render me capable (and appropriately naive(or insane)) to) devote the
  bulk of my effort and concentration to domain specific implementations, with (obviously) very little general appeal, such as this one. 
  *It's either this or rap.
  I'm serious, even my swaggers got a middle finger, and it writes backwards; saving me the trouble of carrying mirrors with me wherever I go! If you've read this far,
  you should have already guessed this; there are no wrong answers here, except for the ones I've written down. And for those paragraphe hoppers, you already know what I'm talkin' about,
  dont you!? Don't think I didn't notice.*
  
  1)
  The other part of repss is the code. A look at the code should give one insight into the sort of decisions that
  were made along the way. Decisions that for the most part are ecountered (in one way or the other) when implementing
  any sort of compiler, in general. So I think for this reason, it's very useful, and light reading when compared to compilers 
  for larger languages like Python (which would be a good reference too I guess if one were dedicated enough). My limited experience
  with compiler courses offered at some institutions (...) were awful for the amount of time that I was enrolled. I knew it wasn't worth
  it's weight in much of anything after the prof skipped lexing, almost entirely, literally saying there are already tools out there that
  are sufficient. I silently disagreed very, and started working on this -- there's a good (valid) argument that I might have benefited from
  staying with the class, as it still covered topics on compiling such as: ASTs, analysis, optimization, and generation; though this argument,
  in my mind, completely ignores reality. Theres a great deal of details outside the theory, that no one book can cover; I don't claim
  to be an expert when it comes to all of them, but I don't think a 4 credit course would have helped much their either. This is my proof
  of sorts. (Obviously, it's still not done.) (I'd like to be graded now please.) I just don't like being held back by language(s),
  so (in true form :like a boss) there are very feww limitations here to speak of (at least not in the specification). The comments are
  meant to be helpful, and my code clear. Whether this is true (as I wouldn't know), let me assure you that I did (at one point) make a conscious
  effort to write for others, and not just for myself when I revisit some time way into the future (as I stumble in drunk one evening, alone, to contemplate my life). So, in theory, other programmers, with
  an appropriately thourough background in the basic data structures (stacks, maps, trees, etc) should have no problem understanding it.
  On a similar note, seeing as how there are usually many ways to do something, I almost always note why I made a decision to go any one way
  where a fork in the road exists and it may be unclear to the reader which one (of a few or a plethora of options) is the better path to be taken
  (counting ones chickens before their hatched). Don't expect me to do so when it's trivially obvious why one option should be chosen over another
  (what more  not caring to elaborate). The code was always designed on some levels with speed in mind, but only after a high level "flexibility" and
  "expresiveness" had been establshed. The overall concept (lexing+analysis+generation indep.) is fairly complex - without my complicating it (help) - so a well-designed
  backend architecture to tie it all together was necessary - if it isn't (re:wasn't) the most important thing, period [note 1].
  In my view, as long as the principle ideas are sound, I like to think that, if you have a well defined structure from the start(beginning),
  most - if not everything else - will just fall into place.

  (NOTE: I should add that it may /[(*:|have::)] just /[(: :|::(as easily)::)] /[(:: spiralled out of control in it's descent toward|:have all went to)] hell (very quickly) if it hadn't!) 
  
  Some notes on the Technical Specifications
  ------------------------------------------
  The organization of the project was architected to be as simple as possible; split into seperate independant modules,
  while keeping things consistent, and available only when and where their needed. I should add that the way we accomplish
  this is through a sub-project whose source is entirely indepedant from the main project. A common theme here (if you catch my drift...)
  is 'meta'. I don't like to consider Repss as a meta-language, nor do I want to consider repss as a translator; both are arguably just that,
  but I believe my argument to the contrary holds more weight, at least here in this tiny locality of textual-processing. My point here is to
  convince you that compilers are important, and the world didn't end with lisp, as some are so adament - though I do love lisp.
  I actually learnt lisp AFTER I finished much of the project, ironically. Picking it up was easy, because I dont think it's true
  to say once youve learnt one language youve learnt them all, but more true to say that once you've created from top to bottom a compiler
  for one language, you've pretty much done them all. I'm not sure, but I think here I do it twice, if not thrice...
  





Footnotes:

[1] *maybe just the mediocre writers like myself...b/c I hear most good writers, the ones worth reading, can write an entire
   novel over the course of a weekend; just one continuous draft written in ink, preferably by feather, without even a hint
   of inspiration - because they have incredably long fingers or a sort of visible abnormality(disfigurement) that makes 
   them quite different from most people. They're monsters/[(**.|;/[(|**they're)] mutants, /[(/[(|**maybe)] even /[(|**a)]
   | we should count them /[(as | a)>><])] diferent species!)]  I don't know about you, but I'm f***ing *jealous!*




