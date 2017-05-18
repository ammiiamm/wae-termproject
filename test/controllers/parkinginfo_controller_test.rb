require 'test_helper'

class ParkinginfoControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers

  test "the truth" do
    assert true
  end

  test "add slot" do
    sign_in users(:one)
    get 'addslot',     zone_id: '1',name: 'p', status: 'false', point1: '(0,0)', point2: '(20,10)', point3: '(30,40)', point4: '(50,60)'
    assert_response :success
  end

  test "free slots" do
    sign_in users(:one)
    get 'totalfree',  id: '1'
    assert_response :success
  end

  test "free slots with time" do
    sign_in users(:one)
    get 'free',  id: '1'
    assert_response :success
  end
end
