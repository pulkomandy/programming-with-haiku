.PHONY = docbooks clean

BUILD = build
MAKEFILE = Makefile
OUTPUT_FILENAME = ProgrammingHaiku
METADATA = metadata.yml
TOC = --toc --toc-depth=2
IMAGES_FOLDER = images
IMAGES = $(IMAGES_FOLDER)/*
COVER_IMAGE = $(IMAGES_FOLDER)/cover.jpg
LATEX_CLASS = book
PDF_ENGINE=xelatex
MATH_FORMULAS = --webtex
CSS_FILE = ../style.css
CSS_ARG = --css=$(CSS_FILE)
METADATA_ARG = --metadata-file=$(METADATA)
ARGS = $(TOC) $(MATH_FORMULAS) $(CSS_ARG) $(METADATA_ARG)
MARKDOWN_FORMAT = markdown-pipe_tables+auto_identifiers+backtick_code_blocks+fancy_lists+table_captions

all: book

book: epub html pdf mobi

clean:
	rm -rf $(BUILD)

CHAPTERS_FOLDER = chapters
CHAPTERS := lesson1.Md lesson2.Md lesson3.Md lesson4.Md  \
	lesson5.Md lesson6.Md lesson7.Md lesson8.Md \
	lesson9.Md lesson10.Md lesson11.Md lesson12.Md \
	lesson13.Md lesson14.Md lesson15.Md lesson16.Md \
	lesson17.Md lesson18.Md lesson19.Md lesson20.Md \
	lesson21.Md lesson22.Md lesson23.Md

CHAPTERS_TARGETS = $(addprefix $(CHAPTERS_FOLDER)/,$(CHAPTERS))

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
	cp -R $(IMAGES_FOLDER)/ $(BUILD)/html/$(IMAGES_FOLDER)/
	cp $(CSS_FILE) $(BUILD)/html/$(CSS_FILE)

$(BUILD)/pdf/$(OUTPUT_FILENAME).pdf:  $(CHAPTERS_TARGETS) $(CHAPTERS_TARGETS) $(CSS_FILE)
	mkdir -p $(BUILD)/markdown
	mkdir -p $(BUILD)/pdf
	pandoc $(ARGS) --metadata=abstract:" " -V documentclass=$(LATEX_CLASS) --pdf-engine=$(PDF_ENGINE) -o $@ -f $(MARKDOWN_FORMAT) -s $(CHAPTERS_TARGETS)

%.xml : %.adoc
	asciidoctor -v -d book -b docbook -s $< -o $@

$(CHAPTERS_FOLDER)/%.Md : %.xml
	mkdir -p $(CHAPTERS_FOLDER)
	pandoc --highlight-style=pygments --wrap=none -f docbook -t $(MARKDOWN_FORMAT) --shift-heading-level-by=1 $< -o $@

$(CHAPTERS_FOLDER)/%.Md : %.odt
	mkdir -p $(CHAPTERS_FOLDER)
	pandoc --highlight-style=pygments --wrap=none -t $(MARKDOWN_FORMAT) --shift-heading-level-by=1 $< -o $@
