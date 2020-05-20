.PHONY = docbooks clean

BUILD = build
MAKEFILE = Makefile
OUTPUT_FILENAME = ProgrammingHaiku
METADATA = metadata.yml
TOC = --toc --toc-depth=2
IMAGES_DIR = images
IMAGES = $(IMAGES_DIR)/*
COVER_IMAGE = $(IMAGES_DIR)/cover.jpg
LATEX_CLASS = book
PDF_ENGINE=xelatex
MATH_FORMULAS = --webtex
CSS_FILE = ../style.css
CSS_ARG = --css=$(CSS_FILE)
METADATA_ARG = --metadata-file=$(METADATA)
ARGS = $(TOC) $(MATH_FORMULAS) $(CSS_ARG) $(METADATA_ARG)
MARKDOWN_FORMAT = markdown-pipe_tables+backtick_code_blocks+fancy_lists+table_captions+smart
CHAPTERS_DIR = chapters
INCLUDE_DIR = ../include

all: book

book: epub html pdf mobi

clean:
	rm -rf $(BUILD)
	rm -rf $(CHAPTERS_DIR)

CHAPTERS := lesson01.Md lesson02.Md lesson03.Md lesson04.Md  \
	lesson05.Md lesson06.Md lesson07.Md lesson08.Md \
	lesson09.Md lesson10.Md lesson11.Md lesson12.Md \
	lesson13.Md lesson14.Md lesson15.Md lesson16.Md \
	lesson17.Md lesson18.Md lesson19.Md lesson20.Md \
	lesson21.Md lesson22.Md lesson23.Md

CHAPTERS_TARGETS = $(addprefix $(CHAPTERS_DIR)/,$(CHAPTERS))

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

$(BUILD)/pdf/$(OUTPUT_FILENAME).pdf:  $(CHAPTERS_TARGETS) $(CHAPTERS_TARGETS) $(CSS_FILE)
	mkdir -p $(BUILD)/markdown
	mkdir -p $(BUILD)/pdf
	pandoc $(ARGS) \
			--template=$(INCLUDE_DIR)/pandoc/eisvogel_mod.latex \
			--metadata=abstract:" " \
			-V documentclass=$(LATEX_CLASS) \
			-V geometry:a4paper \
    		-V toc-title='Table of contents' \
    		-V titlepage:true \
			-V titlepage-color:"FFFFFF" \
			-V titlepage-text-color:"000000" \
			-V titlepage-rule-color:"CCCCCC" \
			-V titlepage-rule-height:4 \
			-V geometry:"left=2.54cm,right=2.54cm,top=1.91cm,bottom=1.91cm" \
			-V subtitle:"Lorem ipsum dolor sic amet" \
			-V links-as-notes:true \
 			-V lot:true \
 			-V logo:$(INCLUDE_DIR)/haiku/haikulogov2.png \
 			-V logo-width:200 \
 			-M date="`date "+%d %B %Y"`" \
    		--listing \
    		--number-section \
			--pdf-engine=$(PDF_ENGINE) \
			--include-in-header $(INCLUDE_DIR)/latex/chapter_break.tex \
			--include-before-body $(INCLUDE_DIR)/latex/before_body.tex \
			--include-after-body $(INCLUDE_DIR)/latex/after_body.tex \
			-o $@ -f $(MARKDOWN_FORMAT) \
			-s $(CHAPTERS_TARGETS)

%.xml : %.adoc
	asciidoctor -v -d book -b docbook -s $< -o $@

$(CHAPTERS_DIR)/%.Md : %.xml
	mkdir -p $(CHAPTERS_DIR)
	pandoc --highlight-style=pygments --wrap=none -f docbook -t $(MARKDOWN_FORMAT) --shift-heading-level-by=1 $< -o $@

