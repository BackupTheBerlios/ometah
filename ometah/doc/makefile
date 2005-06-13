SPLIT="+1"


all: png eps tex

png: interface_main.dia classes_main.dia principles_main.dia
	dia -t png interface_main.dia
	dia -t png classes_main.dia
	dia -t png principles_main.dia

eps: interface_main.dia classes_main.dia principles_main.dia
	dia -t eps interface_main.dia
	dia -t eps classes_main.dia
	dia -t eps principles_main.dia

tex: coding_guidelines.tex roadmap.tex user_guide.tex
	latex coding_guidelines.tex && dvips coding_guidelines.dvi -o coding_guidelines.ps
	latex roadmap.tex && dvips roadmap.dvi -o roadmap.ps
	latex user_guide.tex && dvips user_guide.dvi -o user_guide.ps

html: coding_guidelines.tex roadmap.tex user_guide.tex
	latex2html -split $(SPLIT) coding_guidelines.tex
	latex2html -split $(SPLIT) roadmap.tex
	latex2html -split $(SPLIT) user_guide.tex
