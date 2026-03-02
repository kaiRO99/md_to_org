# md_to_org 

Covert markdown files to Org-mode.

./md_to_org filename.md targetfilename.org

## TODO
- [ ] Tests for files 
- [x] Tests for replace_all()
- [x] Fix table of content placement 
- [ ] Fix args 
  - [ ] Source filename - use for target filename 
  - [ ] Source file does not exist
  - [ ] Source and target filenames
  - [ ] Target file already exists
- [ ] Change name to md2org
- [ ] Enforce space between codeblock start tag and language 
- [ ] Nested emphasis chars: text*text`code*code` (should not trigger bold conversion).

## Roadmap

- [ ] Handle nested emphasis
- [ ] Handle Org-mode table alignment (uses <r>, <l>, <c> in row below separator)
- [ ] Convert single file 
- [ ] Convert entire directory - maintain structure - set destination
- [ ] Convert multiple files and merge (keep name of first)
- [ ] Include an auto-table of contents flag -t - adds a TOC "table of contents :TOC:" to the file.
- [ ] Include flag to ignore properties. 
- [ ] Use threading to handle multiple files or even parsing a file.
- [ ] Handle nested blockquotes?.
- [ ] Add Author Prop with user input.
- [ ] Add title Prop with filename ('_'->' ', capitalize, remove extension).

## Bugs
- Overlapping emphaisis in markdown creates issues.
- Table separator conversion fails when there is whitespace after last pipe "| \n".
- Lists using the "*" character become a level 1 header in Org-mode.

## Notes
- Tables require user to press tab to auto adjust width 

 LSAN_OPTIONS=verbosity=1 ./bin/md_to_org ./tests/test.md ./tests/test.org
