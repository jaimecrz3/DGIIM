# Archive Integrity

[portfolio-migration.tsv](portfolio-migration.tsv) records the original academic archive's file paths, current paths, Git blob IDs and dispositions. It provides a content baseline for checking that historical coursework remains intact. `preserved` identifies retained academic contents; `documentation-rewritten` identifies the six original READMEs; `removed-build-artifact` identifies disposable compiler/CMake files excluded from the archive.

The baseline covers the original tracked files, rather than subsequent documentation or the local untracked Android repository. Run the [validation script](../tools/validate-portfolio.ps1) to check retained contents, local documentation links and notebook JSON. Execution prerequisites and limitations are documented in each project's README.
