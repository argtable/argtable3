################################################################################
# This file is part of the argtable3 library.
#
# Copyright (C) 2014-2019 Tom G. Huang
# <tomghuang@gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of STEWART HEITMANN nor the  names of its contributors
#       may be used to endorse or promote products derived from this software
#       without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL STEWART HEITMANN BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
################################################################################

MKDIR    = mkdir -p
RM       = rm -rf
MV       = mv
CP       = cp
CD       = cd
ZIP      = zip
TAR      = tar
PUSHD    = pushd
POPD     = popd
MKINDEX  = makeindex
PRINTF   = printf
GIT      = git

BUILD_DIR   = build
ARCHIVE_DIR = .archive
TAGS_DIR    = .tags

MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIR := $(abspath $(dir $(MAKEFILE_PATH)))


.PHONY: help
help:
	@printf "Usage: make <target> [options]\n"
	@printf "  make co TAG=<TAG_NAME>      : checkout the specified tag\n"
	@printf "  make archive TAG=<TAG_NAME> : create src archive in the .archive directory\n"
	@printf "  make tag TAG=<TAG_NAME>     : tag the master branch\n"
	@printf "  make taglist                : list all available tags\n"
	@printf "  make cleanall               : clean the distribution package and temp files\n"
	@printf "  make help                   : display this message\n"
	@printf "  make githead                : show the first 7-digit of the HEAD commit SHA-1\n"
	@printf "\n"
	@printf "The <TAG_NAME> format is: v<MAJOR>.<MINOR>.<PATCH>.<BUILD>. The <BUILD> field is the\n"
	@printf "first 7-digit of the HEAD commit SHA-1, which you can get by running 'make githead'.\n"
	@printf "Here are some <TAG_NAME> examples (use 'make taglist' to get all available tags):\n"
	@printf "  v0.0.1.189221c\n"
	@printf "  v1.1.0.bbf3b42\n"
	@printf "  v2.0.0.3b42778\n"


.PHONY: co
co:
	@$(MKDIR) $(ARCHIVE_DIR)
	@$(MKDIR) $(ARCHIVE_DIR)/$(TAG)
	@$(PRINTF) "Export tag %s...\n" $(TAG)
	@$(GIT) -c core.autocrlf=true archive $(TAG) | $(TAR) -x -C $(ARCHIVE_DIR)/$(TAG)
	@$(MKDIR) $(TAGS_DIR)
	@$(MV) $(ARCHIVE_DIR)/$(TAG) $(TAGS_DIR)/$(TAG)
	@$(PRINTF) $(TAG) > $(TAGS_DIR)/$(TAG)/version.tag


.PHONY: archive
archive:
	@$(MKDIR) $(ARCHIVE_DIR)
	@$(MKDIR) $(ARCHIVE_DIR)/argtable-$(TAG)
	@$(PRINTF) "Archive tag %s in %s...\n" $(TAG) $(ARCHIVE_DIR)/argtable-$(TAG).zip
	@$(GIT) -c core.autocrlf=true archive $(TAG) | $(TAR) -x -C $(ARCHIVE_DIR)/argtable-$(TAG)
	@$(PRINTF) $(TAG) > $(ARCHIVE_DIR)/argtable-$(TAG)/version.tag
	@$(CD) $(ARCHIVE_DIR); $(ZIP) -rq argtable-$(TAG).zip argtable-$(TAG)
	@$(RM) $(ARCHIVE_DIR)/argtable-$(TAG)

	@$(MKDIR) $(ARCHIVE_DIR)/argtable-$(TAG)
	@$(PRINTF) "Archive tag %s in %s...\n" $(TAG) $(ARCHIVE_DIR)/argtable-$(TAG).tar.gz
	@$(GIT) archive $(TAG) | $(TAR) -x -C $(ARCHIVE_DIR)/argtable-$(TAG)
	@$(PRINTF) $(TAG) > $(ARCHIVE_DIR)/argtable-$(TAG)/version.tag
	@$(CD) $(ARCHIVE_DIR); $(TAR) -zcf argtable-$(TAG).tar.gz argtable-$(TAG)
	@$(RM) $(ARCHIVE_DIR)/argtable-$(TAG)


.PHONY: tag
tag:
	@$(GIT) tag -a $(TAG) -m "tagging: $(TAG)"


.PHONY: taglist
taglist:
	@$(PRINTF) "Available tags:\n"
	@$(GIT) tag -l


.PHONY: cleanall
cleanall:
	@$(PRINTF) "Clean the distribution package and temp files...\n"
	@$(RM) $(BUILD_DIR)


.PHONY: githead
githead:
	@$(GIT) rev-parse --short=7 HEAD
