# n - take notes
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
  where:
       -g grep notes for all "regx"
       -t print tail of notes, default 20 lines
       -v edit notes
  macros:
       @pwd current directory
  also, multiple-line input from stdin:
       program | n
