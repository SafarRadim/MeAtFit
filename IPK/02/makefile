PROJECT=./IPK-2-Projekt/IPK-2-Projekt.csproj
SOLUTION=./IPK-2-Projekt.sln
RID=linux-x64

#Hand in
handin:
	dotnet publish $(PROJECT) -p:"PublishSingleFile=true;AssemblyName=ipk-l2l3-scan" -r $(RID) --no-self-contained -c Release -o .

release:
	dotnet build $(SOLUTION) -c Release

build:
	dotnet build $(SOLUTION)

log:
	git log --oneline --decorate --pretty="- <\`%h\`> %as %s" --reverse > CHANGELOG.md

clear:
	rm ipk-l2l3-scan ipk-l2l3-scan.pdb
	dotnet clean $(PROJECT)

clean: clear
