# Given(/^I am an admin$/) do
#   @testadminrole = FactoryGirl.create :adminrole
#   @testadmin = FactoryGirl.create :admin
#   @testuserrole = FactoryGirl.create :userrole
#   @testuser = FactoryGirl.create :user
#   @testbanrole = FactoryGirl.create :banrole
#   @testban = FactoryGirl.create :ban
# end
# visit '/users/sign_in'
# fill_in "Username", :with => "testadmin"
# fill_in "Password", :with => "password"
# click_button 'Sign In'
# visit '/welcome/index'
#expect(page).to have_css("img[src*='batch_batch_user.jpg']")

And(/^I should see an image for clicking to site management page$/) do
  expect(page).to have_content "Site Management"
end

When(/^I go to site management page$/) do
  visit '/sites'
end
Then(/^I should see New site link/) do
  expect(page).to have_content "New Site"
end
And(/^I click New site link/) do
  #click_button 'New Site'
  find_link('New Site', href:new_site_path).click
end
When(/^I visit the new site page$/) do
  expect(page).to have_content "New Site"
  expect(page).to have_button "Submit"
end
Then(/^I should be able to fill in new site information$/) do
  fill_in "Name", :with => "Asian Institute of Technology"
  fill_in "Address", :with => "58 Moo 9, Km. 42, Paholyothin Highway Khlong Nung, Pathum Thani 12120"
  fill_in "Districs", :with => "Khlong Nung"
  fill_in "City", :with => "Pathum Thani"
  fill_in "Postalcode", :with => "12120"
end
And(/^I can click submit information$/) do
  click_button "Submit"
end



