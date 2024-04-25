# AUTOMAKE_OPTIONS = foreign

SUBDIRS = include src lib
dist_doc_DATA = README.md
include_HEADERS = include/argparse.h $(LIBFT_PATH)/libft.h

libft: $(LIBFT_PATH)/libft.a
	$(MAKE) -C $(LIBFT_PATH)