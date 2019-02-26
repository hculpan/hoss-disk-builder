# hoss-disk-builder
A disk builder for HOSS

Usage: hdb [-is] <filename>
    -i <filename>   Input file for the boot sector(s)
    -s <sectors>    Number of sectors to write (default: 2880)

## BUILDING
In root directory of project:
    make

## INSTALLATION
In root directory of project:
    make install

This will copy the hdb executable to /usr/local/bin.  This requires 
that you have sudo access on your system.