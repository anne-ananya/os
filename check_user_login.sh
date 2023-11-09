
check_user_logged_in() {
    
    if who | grep -wq "$1"; then
        echo "User $1 is currently logged in."
    else
        echo "User $1 is not logged in."
    fi
}

read -p "Enter the username to check: " username

check_user_logged_in "$username"
