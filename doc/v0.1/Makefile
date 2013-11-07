all: imgs/overview.png
	pdflatex -shell-escape report.tex

imgs:
	mkdir -p imgs

imgs/overview.png: | imgs
	wget http://halfr.net/pub/42sh/overview.png -O $@

p: all
	zathura report.pdf

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
