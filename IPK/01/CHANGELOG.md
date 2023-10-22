# Changelog
A list of changes, updated with each major change.

## Human friendly
### 2023-03-05
- Basic ARG parsing
### 2023-03-08
- Added Connection manager
- Handling Sigint
### 2023-03-18
- UDP connection and handling
- Refactored UDP to use UdpClient instead of raw sockets
### 2023-03-19
- Added TCP textual support
- Correct handling of BYE message
- Fixed newline in UDP mode
- Added `makefile`
- Error handling
### 2023-03-20
- Added non-fatal warnings
### 2023-03-21
- Added makefile
- Renamed folder to properly build binary in project root
- README change
- Changelog added

## Commit list
- <`aa33d9e`> 2023-03-04 Initial commit
- <`5e012da`> 2023-03-04 feat(project)!: Added .gitignore
- <`9732af6`> 2023-03-04 feat(project)!: Added solution
- <`7b3aad2`> 2023-03-05 feat(project)!: folder renamed (IPK-1-Projekt -> ipkcpc)
- <`d751f3d`> 2023-03-05 feat(arguments)!: Basic argument parsing
- <`dd2552d`> 2023-03-05 fix(project): finished renaming things
- <`baba45d`> 2023-03-08 feat(connection)!: Added Connection manager
- <`ec05f77`> 2023-03-08 fix: Yellow squiggly lines gone
- <`f1aedf2`> 2023-03-08 feat(connection): sigint handler
- <`8444be9`> 2023-03-18 fix(project): correct IP address of local server
- <`17c846c`> 2023-03-18 feat(connection)!: UDP working
- <`6d793c2`> 2023-03-18 feat(connection)!: Refactor to use Udp Client() + prettier code
- <`791bd3f`> 2023-03-19 feat(connection)!: TCP working
- <`4d27000`> 2023-03-19 fix(connection): UDP newline on recieve
- <`ee275fe`> 2023-03-19 Fix(connection): correct BYE handling
- <`fa16913`> 2023-03-19 fix(project): yellow squiggly lines begone
- <`abb0ea9`> 2023-03-19 feat(project)!: makefile
- <`8f9e86e`> 2023-03-19 feat(project): Better error handling features
- <`1848a41`> 2023-03-19 feat(project)!: error handling
- <`eb9a16a`> 2023-03-20 fix(connection): warnings for unexpected responses
- <`7b6b0a1`> 2023-03-20 fix(project): added [warning] header to message
- <`403359d`> 2023-03-21 feat(project)!: renamed folder
- <`0ca7f5d`> 2023-03-21 feat(project): updated makefile to work as expected
- <`ddc9d54`> 2023-03-21 fix(project): updated makefile
- <`a3e223e`> 2023-03-21 fix(connection): changed incoming request handling
- <`9040c08`> 2023-03-21 feat(project): changed README
- <`???????`> 2023-03-21 feat(project): Added changelog
