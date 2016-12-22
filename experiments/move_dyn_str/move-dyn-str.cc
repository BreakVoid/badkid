#include <elfio/elfio.hpp>
#include <iostream>

using namespace ELFIO;

int main(int argc, char *argv[]) 
{
    if (argc != 2 ) {
        std::cout << "Usage: move-dyn-str <elf_file>" << std::endl;
        return 1;
    }
    
    // Create an elfio reader
    elfio elfFile;
    
    // Load ELF data
    if (!elfFile.load(argv[1])) {
        std::cout << "Can't find or process ELF file " << argv[1] << std::endl;
        return 2;
    }

    // Print ELF file properties
    std::cout << "ELF file class    : ";
    if ( elfFile.get_class() == ELFCLASS32 )
        std::cout << "ELF32" << std::endl;
    else
        std::cout << "ELF64" << std::endl;
        
    std::cout << "ELF file encoding : ";
    if ( elfFile.get_encoding() == ELFDATA2LSB )
        std::cout << "Little endian" << std::endl;
    else
        std::cout << "Big endian" << std::endl;
        
    // Print ELF file sections info
    Elf_Half sec_num = elfFile.sections.size();
    std::cout << "Number of sections: " << sec_num << std::endl;
    for ( int i = 0; i < sec_num; ++i ) {
        section* psec = elfFile.sections[i];
        std::cout << "  [" << i << "] "
                  << psec->get_name()
                  << "\t"
                  << psec->get_size()
                  << std::endl;
        // Access to section's data
        // const char* p = elfFile.sections[i]->get_data()
    }
        
    // Print ELF file segments info
    Elf_Half seg_num = elfFile.segments.size();
    std::cout << "Number of segments: " << seg_num << std::endl;
    for ( int i = 0; i < seg_num; ++i ) {
        const segment* pseg = elfFile.segments[i];
        std::cout << "  [" << i << "] 0x" << std::hex
                  << pseg->get_flags()
                  << "\t0x"
                  << pseg->get_virtual_address()
                  << "\t0x"
                  << pseg->get_file_size()
                  << "\t0x"
                  << pseg->get_memory_size()
                  << std::endl;
        // Access to segments's data
        // const char* p = elfFile.segments[i]->get_data()
    }

    section *pdynstr;

    for (int i = 0; i < sec_num; ++i) {
        section *psec = elfFile.sections[i];
        if (psec->get_type() == SHT_STRTAB) {
            std::cout << "Section " << i << " is a STRTAB\n\t name is " << psec->get_name() << std::endl;
            if (psec->get_name() == ".dynstr") {
                pdynstr = psec;
            }
        }
    }

    section *newSection = elfFile.sections.add(".dynstr");
    newSection->set_type( pdynstr->get_type() );
    newSection->set_flags( pdynstr->get_flags() );
    newSection->set_addr_align( pdynstr->get_addr_align() );
    newSection->set_data( pdynstr->get_data(), pdynstr->get_size() );
    newSection->set_link( pdynstr->get_link() );

    string_section_accessor dynstr_acc(newSection);

    static char newLibName[] = "Hello World";
    size_t newStringOff = dynstr_acc.add_string(newLibName);

    elfFile.segments[2]->add_section_index( newSection->get_index(), newSection->get_addr_align() );
    
    elfFile.save(std::string(argv[1]) + ".modified");

    Elf64_Addr newDynStrAddr = newSection->get_address();
    std::cout << "new .dynstr address is " << std::hex << newDynStrAddr << std::endl << std::dec;

    for 

    return 0;
}

