# GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  RisingCore_config = debug
  Tests_config = debug
endif
ifeq ($(config),release)
  RisingCore_config = release
  Tests_config = release
endif

PROJECTS := RisingCore Tests

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

RisingCore:
ifneq (,$(RisingCore_config))
	@echo "==== Building RisingCore ($(RisingCore_config)) ===="
	@${MAKE} --no-print-directory -C . -f RisingCore.make config=$(RisingCore_config)
endif

Tests: RisingCore
ifneq (,$(Tests_config))
	@echo "==== Building Tests ($(Tests_config)) ===="
	@${MAKE} --no-print-directory -C tests -f Makefile config=$(Tests_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f RisingCore.make clean
	@${MAKE} --no-print-directory -C tests -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   RisingCore"
	@echo "   Tests"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"