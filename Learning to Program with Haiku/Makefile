.PHONY = docbooks clean

BUILD = build
MAKEFILE = Makefile
OUTPUT_FILENAME = "Programming_with_Haiku_vol1"

INCLUDE_DIR = ../include
IMAGES_DIR = ./images
IMAGES = $(IMAGES_DIR)/*
COVER_IMAGE = $(IMAGES_DIR)/cover.jpg

METADATA = metadata.yml
TOC = --toc --toc-depth=2 -V toc-title='Table of contents' -V toc-own-page:true
CSS_ARG = --css=$(INCLUDE_DIR)/css/style.css
METADATA_ARG = --metadata-file=$(METADATA)
MATH_FORMULAS = --webtex

PDF_ENGINE=xelatex
ARGS = $(MATH_FORMULAS) $(CSS_ARG) $(METADATA_ARG)

MARKDOWN_FORMAT = markdown-pipe_tables+backtick_code_blocks+fancy_lists+table_captions+smart+fenced_divs

CHAPTERS_DIR = ./chapters
CHAPTERS := lesson01.Md lesson02.Md lesson03.Md lesson04.Md  \
	lesson05.Md lesson06.Md lesson07.Md lesson08.Md \
	lesson09.Md lesson10.Md lesson11.Md lesson12.Md \
	lesson13.Md lesson14.Md lesson15.Md lesson16.Md \
	lesson17.Md lesson18.Md lesson19.Md lesson20.Md \
	lesson21.Md lesson22.Md lesson23.Md answers.Md conclusions.Md

CHAPTERS_TARGETS = $(addprefix $(CHAPTERS_DIR)/,$(CHAPTERS))

all: book

book: epub html pdf mobi

clean:
	rm -rf $(BUILD)

markdown: $(BUILD)/markdown/$(OUTPUT_FILENAME).Md

mobi: $(BUILD)/mobi/$(OUTPUT_FILENAME).mobi

epub: $(BUILD)/epub/$(OUTPUT_FILENAME).epub

html: $(BUILD)/html/$(OUTPUT_FILENAME).html

pdf: $(BUILD)/pdf/$(OUTPUT_FILENAME).pdf

$(BUILD)/markdown/$(OUTPUT_FILENAME).Md: $(CHAPTERS_TARGETS)
	mkdir -p $(BUILD)/markdown
	mdmerge $(CHAPTERS_TARGETS) -o $(BUILD)/markdown/$(OUTPUT_FILENAME).Md

$(BUILD)/mobi/$(OUTPUT_FILENAME).mobi: epub
	mkdir -p $(BUILD)/mobi
	ebook-convert $(BUILD)/epub/$(OUTPUT_FILENAME).epub $(BUILD)/mobi/$(OUTPUT_FILENAME).mobi

$(BUILD)/epub/$(OUTPUT_FILENAME).epub: $(MAKEFILE) $(METADATA) $(CHAPTERS_TARGETS) $(CSS_FILE) $(IMAGES) \
																			 $(COVER_IMAGE)
	mkdir -p $(BUILD)/epub
	pandoc $(ARGS) --epub-cover-image=$(COVER_IMAGE) -f $(MARKDOWN_FORMAT) -t epub -o $@ -s $(CHAPTERS_TARGETS)

$(BUILD)/html/$(OUTPUT_FILENAME).html: $(MAKEFILE) $(METADATA) $(CHAPTERS_TARGETS) $(CSS_FILE) $(IMAGES)
	mkdir -p $(BUILD)/html
	pandoc $(ARGS) --standalone --to=html5 -o $@ $(CHAPTERS_TARGETS)
	cp -R $(IMAGES_DIR)/ $(BUILD)/html/$(IMAGES_DIR)/
	cp $(CSS_FILE) $(BUILD)/html/$(CSS_FILE)

$(BUILD)/pdf/$(OUTPUT_FILENAME).pdf:  $(CHAPTERS_TARGETS) $(CSS_FILE)
	mkdir -p $(BUILD)/markdown
	mkdir -p $(BUILD)/pdf
	pandoc $(ARGS) \
			$(TOC) \
			--template=$(INCLUDE_DIR)/pandoc/eisvogel.latex \
			--metadata=abstract:" " \
			-V geometry:a4paper -V geometry:"left=2.54cm,right=2.54cm,top=1.91cm,bottom=1.91cm" \
			-V titlepage:true -V titlepage-color:"FFFFFF" -V titlepage-rule-height:4 \
			-V logo:$(INCLUDE_DIR)/haiku/haikulogov2.png -V logo-width:200 \
			-V links-as-notes:true \
			-V lot:true \
			-V footnotes-pretty:true \
			--listing -V listings-disable-line-numbers:true \
			-V book:true --top-level-division=chapter \
			--pdf-engine=$(PDF_ENGINE) \
			--include-in-header $(INCLUDE_DIR)/latex/preamble.tex \
			--include-before-body $(INCLUDE_DIR)/latex/before_body.tex \
			--include-after-body $(INCLUDE_DIR)/latex/after_body.tex \
			--filter pandoc-latex-environment \
			--number-offset=1 \
			-f $(MARKDOWN_FORMAT) \
			-s $(CHAPTERS_TARGETS) \
			-o $@

%.xml : %.adoc
	asciidoctor -v -d book -b docbook -s $< -o $@

$(CHAPTERS_DIR)/%.Md : %.xml
	mkdir -p $(CHAPTERS_DIR)
	pandoc --highlight-style=pygments --wrap=none -f docbook -t $(MARKDOWN_FORMAT) --shift-heading-level-by=1 $< -o $@

