# badkid
A virus infected ELF file.
## Main Behavior
- Search ELF file in current working directories and infect the file.
- Release a shared object in `LD_LIBRARY_PATH`(`/lib/x86_64-linux-gun` for Ubuntu x64)
- Infected ELF would need the shared object
- The main behaviour is written in the initialization of the shared object.
- The infected ELF would have a suffix `.infected`

## Assumption
### Root Privilege
The executable file is run with root privilege.
### Architecture
x86_64

