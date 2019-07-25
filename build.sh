arduino-cli compile --fqbn arduino:avr:uno src/arpeggiator.ino --output arpeggiator

read -r -p "Emulate ? [Y/n]" response
response=${response,,} # tolower
if [[ $response =~ ^(yes|y| ) ]] || [[ -z $response ]]; then
  emulino ./arpeggiator.hex
fi
