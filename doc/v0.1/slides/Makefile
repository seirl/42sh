all: gantt.png
	pdflatex -shell-escape slides.tex

p: all
	zathura slides.pdf

gantt.png:
	wget http://halfr.net/pub/42sh/gantt.png

clean:
	@rm -rf \
		*.pdf \
		*~ \
		*.backup \
		*.aux \
		*.toc \
		*.blg \
		*.log \
		*.out \
		*.bbl \
		*.dvi \
		*.nav \
		*.pyg \
		*.snm \
		*.lof >/dev/null 2>&1
