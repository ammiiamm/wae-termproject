require 'test_helper'

class UsersControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers

  test "User Status page as guest" do
    get 'status'
    assert_response 302
  end

  test "Make user an admin without authorization" do
    sign_in users(:three)
    get 'change', user: 2, role: 4
    assert_response 302
  end

  test "Give user permision to a site" do
    sign_in users(:one)
    get 'sitepermission', user: 2, site: 1, val: 0, do: 0
    assert_response :success
    get 'sitepermission', user: 2, site: 1, val: 0, do: 1
    assert_response :success
    get 'sitepermission', user: 2, site: 1, val: 0, do: 2
    assert_response :success
  end
end
