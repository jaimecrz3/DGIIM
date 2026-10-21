# Repository Validation

From the repository root on Windows:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tools/validate-portfolio.ps1
```

The script checks tracked Markdown links and image references for local path existence and exact case, parses the eight notebooks, and compares original academic files against the [migration manifest](../docs/portfolio-migration.tsv). It accepts Git checkout line-ending normalization or exact raw baseline bytes. It also checks that the listed disposable build artifacts are absent.

This is a static archive check. It does not fetch external links, validate Markdown anchors, execute notebooks, run project tests or establish that external course frameworks are available. Add new documentation to Git before checking it, because the script uses Git's tracked-file inventory.
