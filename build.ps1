$Target = "MeteoriteCC"
$Test_File = "./MeteoriteSrc/hello.meteor"


# Compiler and Linker flags
$Compiler_Flags = "-std=c++20", "-Wall", "-Wextra", "-Werror", "-Wpedantic", "-Wshadow", "-O2"
$Linker_Flags = "-s"

$Include_Path = "./Src/Include/"

$Src_Files = "compiler", "lexer", "token"
$Build_All = $false


# create obj folder if not exists
if ( !(Test-Path ./Obj) ) {
	mkdir ./Obj
}

# Clear previous build object files if any
Remove-Item ./$Target.exe -Force -ErrorAction SilentlyContinue


# Compiles all src files to obj files
foreach ($src_file in $Src_Files) {

	if ($src_file) {

		$isModified = $true

		# check if the source file is modified

		if ( Test-Path "Obj/${src_file}.o" ) {
			$sourceModifiedDate = (Get-Item "Src/${src_file}.cpp").LastWriteTime
			$targetModifiedDate = (Get-Item "Obj/${src_file}.o").LastWriteTime
			$isModified = ($sourceModifiedDate -gt $targetModifiedDate)
		}

		# compile if source file is modified
		if (($isModified -eq $true) -or ($Build_All -eq $true)) {
			Write-Output "${src_file}.cpp    ${sourceModifiedDate}"

			# remove previous 'obj' file
			Remove-Item Obj/${src_file}.o  -Force -ErrorAction SilentlyContinue
			g++ $Compiler_Flags -I $Include_Path -c Src/${src_file}.cpp -o Obj/${src_file}.o

			# checking if compilation was successful
			if ($LASTEXITCODE -ne 0) {
				Write-Host "Compilation failed!"
				exit 1
			}

		}
	}
}


# Links all the obj files to final executable file
g++ $Linker_Flags Obj/*.o -o ./$Target.exe


if ($LASTEXITCODE -ne 0) {
	Write-Host "Build failed!"
	exit 1
}

# runs the linked executable
& ./${Target}.exe $Test_File
