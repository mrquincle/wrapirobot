#!/bin/make -f

include ../Makefile.defines

install:
	cp *Wrapper.h $(INSTALL_DIR)/include
	echo "Overwrite IRobot.h in $(INSTALL_DIR)/include! For wrapping this is required."
	cp IRobot.h $(INSTALL_DIR)/include

clean:
	@(echo "nothing to clean")
