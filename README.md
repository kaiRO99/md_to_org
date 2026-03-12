# md_to_org 

Convert markdown files to Org-mode format.

### Usage 

```
make
md_to_org [OPTIONS] [TARGET] SOURCE
```

Arguments:</br>
  SOURCE             File or directory to convert</br>
  TARGET             Output file or directory (optional)</br>
  
Options:</br>
  -o, --output PATH  Output path for conversion (file or directory)</br>
  -p, --prop         Includes the properties section, if it exists</br>
  -t, --toc          Includes a table of contents (:toc:) after the first level 1 header</br>
  -r, --recursive    Process directories recursively</br>
  -f, --force        Overwrite existing files</br>
  -d, --dry-run      Show what would be done without doing it</br>
  -h, --help         Show this help message</br>
  
Examples:</br>
  md_to_org input.md                    Convert single file (output: input.org)</br>
  md_to_org -o output.org input.md      Convert with specific output name</br>
  md_to_org docs/                       Convert all .md files in directory</br>
  md_to_org -o out/ docs/               Convert directory docs/ to out/ </br>
  md_to_org -r docs/                    Convert directory recursively</br>

## TODO
- [x] Tests for files 
- [x] Tests for replace_all()
- [x] Fix table of content placement 
- [x] Tests for command line 
- [x] Fix args 
  - [x] Source filename - use for target filename 
  - [x] Source file does not exist
  - [x] Source and target filenames
  - [x] Target file already exists
- [ ] Change name to md2org
- [ ] Enforce space between codeblock start tag and language 
- [ ] Test nested emphasis chars in inline-code: text*text`code*code` (should not trigger bold conversion).
- [ ] Change parse_file() arguments to just config?
- [x] Add check in arg parsing that config.target is .md when case -o, else print use
- [x] Add cli test for recursive, w/ target path

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

## Tests
- test_cli: Test command line options 
- test_utils: Test helper functions 
- test_line: Test line parsing logic
- test_file: Test individual file parsing logic
