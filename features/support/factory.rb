FactoryGirl.define do
  factory :adminrole, class: Role do
    id '1'
    name 'admin'
  end

  factory :admin, class: User do
    id '1'
    email 'testadmin@ait.asia'
    username 'testadmin'
    password 'password'
    password_confirmation 'password'
    role_id '1'
  end

  factory :userrole, class: Role do
    id '2'
    name 'user'
  end

  factory :user, class: User do
    id '2'
    email 'testuser@ait.asia'
    username 'testuser'
    password 'password'
    password_confirmation 'password'
    role_id '2'
  end

  factory :banrole, class: Role do
    id '3'
    name 'banned'
  end

  factory :ban, class: User do
    id '3'
    email 'testban@ait.asia'
    username 'testban'
    password 'password'
    password_confirmation 'password'
    role_id '3'
  end



end