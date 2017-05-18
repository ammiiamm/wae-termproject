require 'test_helper'

class HomographyControllerTest < ActionController::TestCase
  include Devise::Test::IntegrationHelpers
  include Devise::Test::ControllerHelpers

  test "the truth" do
    assert true
  end

  test "homography" do
    post :calchm, task: "2", values: "287.5,161,385.5,258,482.5,202,369.5,117,0,0,100,0,100,30,0,30", divide:  "5"
    assert_response :success
  end
end

