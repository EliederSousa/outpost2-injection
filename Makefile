COMPILER        = C:\mingw32\bin\g++.exe

# Sources of the application. The main file it's the one with DLLMain function.
SOURCES_PATH    = ./src
SOURCES_NAME    = *.cpp
SOURCES_MAIN    = main.cpp

# Let's include some libs.
INCLUDE_PATHS   = -I./src/include/
LIBRARY_PATHS   = -L./src/lib/

# Flags
COMPILER_FLAGS  = -w -Os -g0 -Wl,-subsystem,windows -Wl,--exclude-libs,ALL -Wl,--gc-sections -shared -m32
LINKER_FLAGS    = -s -static-libgcc -static-libstdc++

# Resulting file
OUTPUT_PATH     = ./build
OUTPUT_NAME     = odasl.dll

# Game folder
OUTPOST2_FOLDER = "C:\GOG Games2\Outpost 2\OPU"

# File to record the timestamp of last compilation
TIMESTAMP_FILE  = lastbuild.timestamp

# Main
all:
	@cls && $(COMPILER) $(SOURCES_PATH)/$(SOURCES_NAME) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUTPUT_PATH)/$(OUTPUT_NAME)
	@echo Build completo: $(OUTPUT_PATH)/$(OUTPUT_NAME)

# Timestamp
$(TIMESTAMP_FILE): $(SOURCES_PATH)/$(SOURCES_NAME)
	@powershell -command "Get-Date -Format 'dd/MM/yyyy,HH:mm:ss'" > $(OUTPUT_PATH)/$(TIMESTAMP_FILE)

# Clear all building
clear:
	@cls && echo Limpando a pasta build...
	@if exist "$(OUTPUT_PATH)" del /Q "$(OUTPUT_PATH)\$(OUTPUT_NAME)" "$(OUTPUT_PATH)\$(TIMESTAMP_FILE)"

# Copy routine
copy:
	@cls && echo Copiando para o Outpost 2...
	@if exist "$(OUTPUT_PATH)/$(OUTPUT_NAME)" ( \
		xcopy /F /I /Y "$(OUTPUT_PATH)/$(OUTPUT_NAME)" $(OUTPOST2_FOLDER) \
	)
	@if exist "$(OUTPUT_PATH)/$(TIMESTAMP_FILE)" xcopy /F /I /Y "$(OUTPUT_PATH)/$(TIMESTAMP_FILE)" $(OUTPOST2_FOLDER)
	@echo Copia concluida!

# Build and copy
install: all copy

run: install
	@cd $(OUTPOST2_FOLDER)\.. && start Outpost2.exe /OPU
