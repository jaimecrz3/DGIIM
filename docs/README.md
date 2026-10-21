# Refactor Evidence

[portfolio-migration.tsv](portfolio-migration.tsv) records every file tracked at the pre-refactor baseline, its final path, original Git blob ID and disposition. `preserved` files retain their academic contents; `documentation-rewritten` identifies the six original READMEs; `removed-build-artifact` identifies disposable compiler/CMake files.

The manifest covers the original archive, not newly added documentation or the pre-existing untracked Android repository. Run the [validation script](../tools/validate-portfolio.ps1) to check retained content and local documentation links. See the [refactor report](../PORTFOLIO_REFACTOR_REPORT.md) for limitations and the [migration plan](../PORTFOLIO_REFACTOR_PLAN.md) for decisions.
