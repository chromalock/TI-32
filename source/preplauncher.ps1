node ./prepprog.mjs ./programs/LAUNCHER.8xp ./launcher.var
$bytes = [System.IO.File]::ReadAllBytes(".\launcher.var")
$hex = ($bytes | ForEach-Object { "0x{0:X2}" -f $_ }) -join ", "
$lines = for ($i = 0; $i -lt $hex.Length; $i += 75) {
    "  " + $hex.Substring($i, [Math]::Min(75, $hex.Length - $i))
}
$output = @"
unsigned char __launcher_var[] = {
$($lines -join ",`n")
};
unsigned int __launcher_var_len = $($bytes.Length);
"@
$output | Out-File -Encoding ASCII ".\esp32\launcher.h"
