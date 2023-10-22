IP=172.24.220.28
PORT=2023
SOLUTION=IPK-1-Projekt.sln
PROJECT=./ipkcpc-src/ipkcpc.csproj
RID=linux-x64

# Hand In
handin:
	dotnet publish $(PROJECT) -p:PublishSingleFile=true -r $(RID) --no-self-contained -c Release  -o .

# BUILD
release:
	dotnet build $(SOLUTION) -c Release

build:
	dotnet build $(SOLUTION)

# RUN
udp: build
	dotnet run --project $(PROJECT) -- -h $(IP) -p $(PORT) -m udp

tcp: build
	dotnet run --project $(PROJECT) -- -h $(IP) -p $(PORT) -m tcp

# PUBLISH
publish:
	dotnet publish $(PROJECT)

# CHANGELOG
log:
	git log --oneline --decorate --pretty="- <\`%h\`> %as %s" --reverse > CHANGELOG.md

# CLEANUP
clear:
	dotnet clean $(PROJECT)
	rm ipkcpc ipkcpc.pdb

clean: clear
