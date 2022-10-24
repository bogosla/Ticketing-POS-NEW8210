##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=NEW8210Project
ConfigurationName      :=Release
WorkspacePath          :=C:/Projects/volcano/NEW8210/Dev/NEW8210
ProjectPath            :=C:/Projects/volcano/NEW8210/Dev/NEW8210/NEW8210Project
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Pc
Date                   :=30/01/2019
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/glibc-oabi-toolchain-arm-generic/bin/arm-unknown-linux-gnu-g++.exe
SharedObjectLinkerName :=C:/glibc-oabi-toolchain-arm-generic/bin/arm-unknown-linux-gnu-g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="NEW8210Project.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=windres
LinkOptions            :=  -marm -mapcs -mno-sched-prolog -mabi=apcs-gnu -mlittle-endian -mno-thumb-interwork -msoft-float -Wl,-Map
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)include $(IncludeSwitch)/cygdrive/c/Projects/volcano/NEW8210/Dev/sdk/include $(IncludeSwitch)/cygdrive/c/Projects/volcano/NEW8210/Dev/sdk/include/directfb $(IncludeSwitch)fribidi/include $(IncludeSwitch)seos/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)pos $(LibrarySwitch)directfb $(LibrarySwitch)direct $(LibrarySwitch)fusion $(LibrarySwitch)iccard $(LibrarySwitch)z $(LibrarySwitch)fribidi $(LibrarySwitch)seos $(LibrarySwitch)crypto $(LibrarySwitch)ssl $(LibrarySwitch)curl $(LibrarySwitch)nfc $(LibrarySwitch)ped $(LibrarySwitch)emv 
ArLibs                 :=  "libpos" "libdirectfb" "libdirect" "libfusion" "libiccard" "libz" "libfribidi" "libseos" "libcrypto" "libssl" "libcurl" "libnfc" "libped" "libemv" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/cygdrive/c/Projects/volcano/NEW8210/Dev/sdk/lib $(LibraryPathSwitch)fribidi/lib $(LibraryPathSwitch)seos/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/glibc-oabi-toolchain-arm-generic/bin/arm-unknown-linux-gnu-ar.exe rcu
CXX      := C:/glibc-oabi-toolchain-arm-generic/bin/arm-unknown-linux-gnu-g++.exe
CC       := C:/glibc-oabi-toolchain-arm-generic/bin/arm-unknown-linux-gnu-gcc.exe
CXXFLAGS :=  -O3 -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall --sysroot=/cygdrive/c/Projects/volcano/NEW8210/Dev/glibc-oabi-toolchain-arm-generic/arm-unknown-linux-gnu/sys-root/ -Wundef -Wstrict-prototypes -Werror-implicit-function-declaration -Wdeclaration-after-statement -fsigned-char -marm -mapcs -mno-sched-prolog -mabi=apcs-gnu -mlittle-endian $(Preprocessors)
ASFLAGS  := 
AS       := C:/glibc-oabi-toolchain-arm-generic/bin/arm-unknown-linux-gnu-as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/src_main.c$(ObjectSuffix) $(IntermediateDirectory)/src_ticketing.c$(ObjectSuffix) $(IntermediateDirectory)/src_wireless.c$(ObjectSuffix) $(IntermediateDirectory)/src_visualstring.c$(ObjectSuffix) $(IntermediateDirectory)/src_auth.c$(ObjectSuffix) $(IntermediateDirectory)/cjson_cJSON.c$(ObjectSuffix) $(IntermediateDirectory)/src_string_convert.c$(ObjectSuffix) $(IntermediateDirectory)/src_common.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_main.c$(ObjectSuffix): src/main.c $(IntermediateDirectory)/src_main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/volcano/NEW8210/Dev/NEW8210/NEW8210Project/src/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.c$(DependSuffix): src/main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.c$(DependSuffix) -MM src/main.c

$(IntermediateDirectory)/src_main.c$(PreprocessSuffix): src/main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.c$(PreprocessSuffix) src/main.c

$(IntermediateDirectory)/src_ticketing.c$(ObjectSuffix): src/ticketing.c $(IntermediateDirectory)/src_ticketing.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/volcano/NEW8210/Dev/NEW8210/NEW8210Project/src/ticketing.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ticketing.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ticketing.c$(DependSuffix): src/ticketing.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ticketing.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ticketing.c$(DependSuffix) -MM src/ticketing.c

$(IntermediateDirectory)/src_ticketing.c$(PreprocessSuffix): src/ticketing.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ticketing.c$(PreprocessSuffix) src/ticketing.c

$(IntermediateDirectory)/src_wireless.c$(ObjectSuffix): src/wireless.c $(IntermediateDirectory)/src_wireless.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/volcano/NEW8210/Dev/NEW8210/NEW8210Project/src/wireless.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_wireless.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_wireless.c$(DependSuffix): src/wireless.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_wireless.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_wireless.c$(DependSuffix) -MM src/wireless.c

$(IntermediateDirectory)/src_wireless.c$(PreprocessSuffix): src/wireless.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_wireless.c$(PreprocessSuffix) src/wireless.c

$(IntermediateDirectory)/src_visualstring.c$(ObjectSuffix): src/visualstring.c $(IntermediateDirectory)/src_visualstring.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/volcano/NEW8210/Dev/NEW8210/NEW8210Project/src/visualstring.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_visualstring.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_visualstring.c$(DependSuffix): src/visualstring.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_visualstring.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_visualstring.c$(DependSuffix) -MM src/visualstring.c

$(IntermediateDirectory)/src_visualstring.c$(PreprocessSuffix): src/visualstring.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_visualstring.c$(PreprocessSuffix) src/visualstring.c

$(IntermediateDirectory)/src_auth.c$(ObjectSuffix): src/auth.c $(IntermediateDirectory)/src_auth.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/volcano/NEW8210/Dev/NEW8210/NEW8210Project/src/auth.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_auth.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_auth.c$(DependSuffix): src/auth.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_auth.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_auth.c$(DependSuffix) -MM src/auth.c

$(IntermediateDirectory)/src_auth.c$(PreprocessSuffix): src/auth.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_auth.c$(PreprocessSuffix) src/auth.c

$(IntermediateDirectory)/cjson_cJSON.c$(ObjectSuffix): cjson/cJSON.c $(IntermediateDirectory)/cjson_cJSON.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/volcano/NEW8210/Dev/NEW8210/NEW8210Project/cjson/cJSON.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cjson_cJSON.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cjson_cJSON.c$(DependSuffix): cjson/cJSON.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cjson_cJSON.c$(ObjectSuffix) -MF$(IntermediateDirectory)/cjson_cJSON.c$(DependSuffix) -MM cjson/cJSON.c

$(IntermediateDirectory)/cjson_cJSON.c$(PreprocessSuffix): cjson/cJSON.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cjson_cJSON.c$(PreprocessSuffix) cjson/cJSON.c

$(IntermediateDirectory)/src_string_convert.c$(ObjectSuffix): src/string_convert.c $(IntermediateDirectory)/src_string_convert.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/volcano/NEW8210/Dev/NEW8210/NEW8210Project/src/string_convert.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_string_convert.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_string_convert.c$(DependSuffix): src/string_convert.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_string_convert.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_string_convert.c$(DependSuffix) -MM src/string_convert.c

$(IntermediateDirectory)/src_string_convert.c$(PreprocessSuffix): src/string_convert.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_string_convert.c$(PreprocessSuffix) src/string_convert.c

$(IntermediateDirectory)/src_common.c$(ObjectSuffix): src/common.c $(IntermediateDirectory)/src_common.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/volcano/NEW8210/Dev/NEW8210/NEW8210Project/src/common.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_common.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_common.c$(DependSuffix): src/common.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_common.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_common.c$(DependSuffix) -MM src/common.c

$(IntermediateDirectory)/src_common.c$(PreprocessSuffix): src/common.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_common.c$(PreprocessSuffix) src/common.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


