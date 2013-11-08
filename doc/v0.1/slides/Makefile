all:
	pdflatex -shell-escape slides.tex

p: all
	zathura slides.pdf

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
