#! /bin/bash

cd docs || exit 1
doxygen Doxyfile

read -p "Do you want to start a server (PHP is required)? (yes/no): " response

if [ "$response" = "yes" ]; then
    cd doxygen_documentation/html || exit 1
    php -S localhost:9000
elif [ "$response" = "no" ]; then
    echo "Not starting the server."
else
    echo "Invalid response. Please answer with 'yes' or 'no'."
    exit 1
fi
