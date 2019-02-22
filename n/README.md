# n - take notes
Keep a timestamped log of notes in `~/n/notes.txt`. Quick access to file through `-v` (edit), `-t` (tail). Grep contents with `-g`. Macro expansion of `@pwd` to current directory. Log a command & its output with `-c`.

Link `n` as `nn` to activate local directory storage (i.e. `notes.txt` will be stored in the current directory rather than in `~/n`.
## Build
```
make
```
## Usage
```
n text of note
n -g "regx [regx]"
n -t [lines]
n -v
n -h
n -c cmd-line
n -l cmd-line
program | n
n < file.txt

where:
  -g    - grep notes for all "regx"
  -t    - print tail of notes, default 20 lines
  -v    - edit notes
  -h    - this help message
  -c    - execute cmd-line & note all stdout
  -l    - execute cmd-line, don't note stdout
  stdin - note line created for each stdin line
  
macros:
  @pwd current directory
```
