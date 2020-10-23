if ($env:APPVEYOR_PULL_REQUEST_NUMBER -ne $null) {
    exit
}

$COMMIT_SHORT_SHA = git rev-parse --short HEAD | Out-String
$COMMIT_NUM = git rev-list --count HEAD | Out-String
$COMMIT_SHORT_SHA = $COMMIT_SHORT_SHA.Trim()
$COMMIT_NUM = $COMMIT_NUM.Trim()
$CC_VERSION = $env:APPVEYOR_BUILD_WORKER_IMAGE.Substring($env:APPVEYOR_BUILD_WORKER_IMAGE.Length - 4)

Set-Location -Path $env:APPVEYOR_BUILD_FOLDER\upload
if ($env:APPVEYOR_REPO_TAG_NAME -eq $null) {
    $ARCHIVE_NAME = "spmod-win32-$COMMIT_NUM-$COMMIT_SHORT_SHA-msvc$CC_VERSION-$env:LINK_TYPE.7z"
    7z a -t7z -mm=LZMA:d256m:fb64 -mx9 "$ARCHIVE_NAME" dlls exts scripting
    Push-AppveyorArtifact $ARCHIVE_NAME
} else {
    $ARCHIVE_NAME = "spmod-win32-$env:APPVEYOR_REPO_TAG_NAME-msvc$CC_VERSION-$env:LINK_TYPE.7z"
    7z a -t7z -mm=LZMA:d256m:fb64 -mx9 "$ARCHIVE_NAME" dlls exts scripting
    Push-AppveyorArtifact $ARCHIVE_NAME
}
