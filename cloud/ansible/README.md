# Usage
Ansible vault password goes in .password file that you make yourself, otherwise commands are to be run with --ask-vault-password


## First config of server
Set IP in inventory.ini and set ansible_user to host user (for STRATO it is debian)
```
ansible-playbook -i inventory.ini --tags "useradd"
```

After run, change ansible_user back to ansible

Run the full playbook pipeline
```
ansible-playbook -i inventory.ini
```


## Singular deploy/update existing
```
ansible-playbook -i inventory.ini --tags "tttdeploy"
```