BUILD_DIR = build

all: $(BUILD_DIR)
	cd $(BUILD_DIR); cmake $(CURDIR); make

run: 
	@echo "Starting tests"
	$(BUILD_DIR)/"code"

valgrind: 
	@echo "Starting tests with valgrind"
	cd $(BUILD_DIR); valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./code

rebuild: clean all
	
$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
	
.PHONY: all cmake clean