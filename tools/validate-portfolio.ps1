param([string]$RepositoryRoot = (Split-Path $PSScriptRoot -Parent))
$ErrorActionPreference = 'Stop'
$RepositoryRoot = [IO.Path]::GetFullPath($RepositoryRoot)
Push-Location $RepositoryRoot
try {
    $files = @(git -c core.quotepath=false ls-files)
    if ($LASTEXITCODE -ne 0) { throw 'Cannot read the Git file inventory.' }
    $failures = @()
    $linkCount = 0
    $documents = @($files | Where-Object { $_ -match '\.md$' })
    foreach ($file in $documents) {
        if (!(Test-Path -LiteralPath $file)) { continue } # Staged cleanup may remove files.
        $body = [IO.File]::ReadAllText((Join-Path $RepositoryRoot $file))
        # Inline Markdown links/images and HTML image references. Remote URLs are not fetched.
        $targets = @([regex]::Matches($body, '!?\[[^\]\r\n]*\]\(([^\r\n]*?)\)') | ForEach-Object { $_.Groups[1].Value })
        $targets += @([regex]::Matches($body, '<img\b[^>]*\bsrc=["'']([^"'']+)["'']') | ForEach-Object { $_.Groups[1].Value })
        foreach ($target in $targets) {
            if (!$target -or $target -match '^(?:[a-z][a-z0-9+.-]*:|#|//)') { continue }
            $target = ($target -split '#', 2)[0]
            $decoded = [Uri]::UnescapeDataString($target.Trim('<', '>'))
            $resolved = [IO.Path]::GetFullPath((Join-Path (Split-Path (Join-Path $RepositoryRoot $file)) $decoded))
            $linkCount++
            if (!$resolved.StartsWith($RepositoryRoot + [IO.Path]::DirectorySeparatorChar) -or !(Test-Path -LiteralPath $resolved)) {
                $failures += "$file -> $target"
                continue
            }
            # Check spelling/case component by component, even on a case-insensitive filesystem.
            $cursor = $RepositoryRoot
            foreach ($component in $resolved.Substring($RepositoryRoot.Length + 1).Split([IO.Path]::DirectorySeparatorChar)) {
                if (!$component) { continue }
                $actual = @(Get-ChildItem -LiteralPath $cursor -Force | Where-Object { $_.Name -ceq $component })
                if (!$actual.Count) { $failures += "Case mismatch: $file -> $target"; break }
                $cursor = Join-Path $cursor $component
            }
        }
    }
    $notebooks = @($files | Where-Object { $_ -match '\.ipynb$' })
    foreach ($file in $notebooks) {
        $notebook = Get-Content -LiteralPath $file -Raw -Encoding utf8 | ConvertFrom-Json
        if ($notebook.nbformat -ne 4 -or !$notebook.cells) { $failures += "Invalid notebook: $file" }
    }
    $manifestPath = Join-Path $RepositoryRoot 'docs/portfolio-migration.tsv'
    if (Test-Path -LiteralPath $manifestPath) {
        $manifest = @(Import-Csv -LiteralPath $manifestPath -Delimiter "`t" -Encoding utf8)
        $preserved = @($manifest | Where-Object action -eq 'preserved')
        $missing = @($preserved | Where-Object { !(Test-Path -LiteralPath $_.current_path) })
        if ($missing.Count) { throw "Missing $($missing.Count) original files." }
        $previousEncoding = $OutputEncoding
        try {
            $OutputEncoding = New-Object Text.UTF8Encoding($false)
            $hashes = @($preserved.current_path | git hash-object --stdin-paths)
        } finally { $OutputEncoding = $previousEncoding }
        if ($LASTEXITCODE -ne 0 -or $hashes.Count -ne $preserved.Count) { throw 'Git content-hash validation failed.' }
        for ($i = 0; $i -lt $preserved.Count; $i++) {
            if ($hashes[$i] -ne $preserved[$i].original_blob) {
                # Some original blobs contain CRLF; accept exact raw bytes as well as Git checkout normalization.
                $raw = git hash-object --no-filters -- $preserved[$i].current_path
                if ($raw -ne $preserved[$i].original_blob) { $failures += "Original content changed: $($preserved[$i].current_path)" }
            }
        }
        foreach ($row in ($manifest | Where-Object action -eq 'removed-build-artifact')) {
            if (Test-Path -LiteralPath $row.current_path) { $failures += "Removed build artifact still present: $($row.current_path)" }
        }
        Write-Output "$($preserved.Count) original files match their baseline Git content (allowing checkout line endings)."
    }
    if ($failures.Count) { $failures | Write-Output; throw "$($failures.Count) validation failure(s)." }
    Write-Output "$($documents.Count) Markdown documents checked; $linkCount local link/image targets resolve with exact case; $($notebooks.Count) notebooks parse as nbformat 4."
    Write-Output 'External URLs, Markdown anchors, notebook execution and project builds are outside this static check.'
} finally {
    Pop-Location
}
