# md_to_org 

./md_to_org filename.md targetfilename.org

thoughts:
- use target directory? - make it the same name 
- use target file, and if it already exists, append?


## Roadmap

- [ ] convert single file 
- [ ] convert entire directory - maintain structure - set destination
- [ ] convert multiple files and merge (keep name of first)
- [ ] Include an auto-table of contents flag -t - adds a TOC "table of contents :TOC:" to the file.
- [ ] Include flag to ignore properties 
- [ ] handle *, _ that are not for emphasis
- [ ] use threading to handle multiple files or even parsing a file
- [ ] Handle nested blockquotes
- [ ] Add Author Prop with user input.
- [ ] add title Prop with filename ('_'->' ', capitalize, remove extension)
- [ ] If 1 args -> use same filename, if 2, use 2nd argv

## Bugs
- overlapping emphaisis in .md creates issues
- table separator conversion fails when there is whitespace after last pipe "| \n" 

## Notes
- Table of contents inserted after first H1 section as a H2
- Tables require user to press tab to auto adjust width 

 LSAN_OPTIONS=verbosity=1 ./bin/md_to_org ./tests/test.md ./tests/test.org
