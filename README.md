# md_to_org

./md_to_org filename.md targetfilename.org

thoughts:
- use target directory? - make it the same name 
- use target file, and if it already exists, append?

- tables willr requrie user to press tab on them so they auto adjust 

## Roadmap

- [ ] convert entire directory (dfs) - maintain structure - set destination
- [ ] convert single file 
- [ ] convert multiple files and merge (keep name of first)
- [ ] Include an auto-table of contents flag -t - adds a TOC "table of contents :TOC:" to the file.
- [ ] Include flag to ignore properties 
- [ ] handle *, _ that are not for emphasis
- [ ] use threading to handle multiple files or even parsing a file

## Bugs
- overlapping emphaisis in .md creates issues
