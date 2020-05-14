BUILD = build
MAKEFILE = Makefile
OUTPUT_FILENAME = ProgrammingHaiku
METADATA = metadata.yml
CHAPTERS = chapters/*.md
TOC = --toc --toc-depth=2
IMAGES_FOLDER = images
IMAGES = $(IMAGES_FOLDER)/*
COVER_IMAGE = $(IMAGES_FOLDER)/cover.png
LATEX_CLASS = report
MATH_FORMULAS = --webtex
CSS_FILE = style.css
CSS_ARG = --css=$(CSS_FILE)
METADATA_ARG = --metadata-file=$(METADATA)
ARGS = $(TOC) $(MATH_FORMULAS) $(CSS_ARG) $(METADATA_ARG)

CDIR := .
ODIR := .
ADOC_SRCS := $(wildcard $(CDIR)/*.adoc)
ADOC_OBJS := $(SRCS:$(CDIR)/%.adoc=$(ODIR)/%.xml)

all: book

book: epub html pdf mobi

clean:
	rm -r $(BUILD)

DOCBOOKS  := lesson1.adoc lesson2.adoc lessob3.adic
    
docbooks: $(OBJECTS)
	

%.xml : %.adoc
		asciidoc -b docbook lesson1.adoc 

mobi: $(BUILD)/mobi/$(OUTPUT_FILENAME).mobi

epub: $(BUILD)/epub/$(OUTPUT_FILENAME).epub

html: $(BUILD)/html/$(OUTPUT_FILENAME).html

pdf: $(BUILD)/pdf/$(OUTPUT_FILENAME).pdf

$(BUILD)/mobi/$(OUTPUT_FILENAME).mobi: epub
	mkdir -p $(BUILD)/mobi
	ebook-convert $(BUILD)/epub/$(OUTPUT_FILENAME).epub $(BUILD)/mobi/$(OUTPUT_FILENAME).mobi

$(BUILD)/epub/$(OUTPUT_FILENAME).epub: $(MAKEFILE) $(METADATA) $(CHAPTERS) $(CSS_FILE) $(IMAGES) \
																			 $(COVER_IMAGE)
	mkdir -p $(BUILD)/epub
	pandoc $(ARGS) --epub-cover-image=$(COVER_IMAGE) -o $@ $(CHAPTERS)

$(BUILD)/html/$(OUTPUT_FILENAME).html: $(MAKEFILE) $(METADATA) $(CHAPTERS) $(CSS_FILE) $(IMAGES)
	mkdir -p $(BUILD)/html
	pandoc $(ARGS) --standalone --to=html5 -o $@ $(CHAPTERS)
	cp -R $(IMAGES_FOLDER)/ $(BUILD)/html/$(IMAGES_FOLDER)/
	cp $(CSS_FILE) $(BUILD)/html/$(CSS_FILE)

$(BUILD)/pdf/$(OUTPUT_FILENAME).pdf: $(MAKEFILE) $(METADATA) $(CHAPTERS) $(CSS_FILE) $(IMAGES)
	mkdir -p $(BUILD)/pdf
	pandoc $(ARGS) -V documentclass=$(LATEX_CLASS) -o $@ $(CHAPTERS)
