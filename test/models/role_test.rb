require 'test_helper'

class RoleTest < ActiveSupport::TestCase

  test "should validate presence of role" do
    role = Role.new
    assert !role.valid?
    assert_equal ["can't be blank"], role.errors[:name]
  end

  test "should validate uniqueness of roles" do
    old_role = roles(:one)
    role = Role.new name: old_role.name
    assert !role.valid?
    assert_equal ["has already been taken"], role.errors[:name]
  end
end
