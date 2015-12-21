CC=gcc
headName=h
srcName=c
CFLAGS=-Wall -DDEBUG
EFLAGS=-lpthread -g

server=server.out
client=client.out

nameObjectDir=bin
nameObjectSrc=src

objectDirClient=$(shell pwd)/client/$(nameObjectDir)
objectDirServer=$(shell pwd)/server/$(nameObjectDir)
objectDirData=$(shell pwd)/data/$(nameObjectDir)
srcDirServer=$(shell pwd)/server/$(nameObjectSrc)
srcDirClient=$(shell pwd)/client/$(nameObjectSrc)
srcDirData=$(shell pwd)/data/$(nameObjectSrc)

directoriesServer=$(wildcard $(srcDirServer)/*/)
directoriesClient=$(wildcard $(srcDirClient)/*/)
directoriesData=$(wildcard $(srcDirData)/*/)

srcFileServer=$(foreach dir, $(directoriesServer), $(wildcard $(dir)*.$(srcName)))
srcFileClient=$(foreach dir, $(directoriesClient), $(wildcard $(dir)*.$(srcName)))
srcFileData=$(foreach dir, $(directoriesData), $(wildcard $(dir)*.$(srcName)))

headersServer=$(wildcard $(directoriesServer)*.$(headName)) $(wildcard $(directoriesData)*.$(headName))
headersClient=$(wildcard $(directoriesClient)*.$(headName)) $(wildcard $(directoriesData)*.$(headName))

objectsServer=$(foreach file, $(srcFileServer:.$(srcName)=.o), $(objectDirServer)/$(notdir $(file)))
objectsClient=$(foreach file, $(srcFileClient:.$(srcName)=.o), $(objectDirClient)/$(notdir $(file)))
objectsData=$(foreach file, $(srcFileData:.$(srcName)=.o), $(objectDirData)/$(notdir $(file)))

all: $(server) $(client)

$(server): compilationServer
	@$(CC) -o $(server) $(objectsServer) $(objectsData) $(EFLAGS)

$(client): compilationClient
	@$(CC) -o $(client) $(objectsClient) $(objectsData) $(EFLAGS)

compilationServer:
	@mkdir $(objectDirServer) -p
	@mkdir $(objectDirData) -p
	@for dir in $(directoriesServer) $(directoriesData); do \
		if [ ! -e $$dir/Makefile ]; then \
				cp Makefile_type $$dir/Makefile; \
		fi; \
		make --no-print-directory -C $$dir objectDir="../../$(nameObjectDir)" headers="$(headersServer)" CC="$(CC)" headName="$(headName)" srcName="$(srcName)" CFLAGS="$(CFLAGS)";	\
	done


compilationClient:
	@mkdir $(objectDirClient) -p
	@mkdir $(objectDirData) -p
	@for dir in $(directoriesClient) $(directoriesData); do \
		if [ ! -e $$dir/Makefile ]; then \
				cp Makefile_type $$dir/Makefile; \
		fi; \
		make --no-print-directory -C $$dir objectDir=../../$(nameObjectDir) headers="$(headersClient)" CC="$(CC)" headName="$(headName)" srcName="$(srcName)" CFLAGS="$(CFLAGS)";	\
	done

clean_all: clean
	rm $(server) $(client)
	find data/ -name Makefile -exec rm {} +
	find server/ -name Makefile -exec rm {} +
	find client/ -name Makefile -exec rm {} +

clean:
	rm -R $(objectDirClient)
	rm -R $(objectDirServer)
	rm -R $(objectDirData)
