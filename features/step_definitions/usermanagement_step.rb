Given(/^I am an admin$/) do
  @testadminrole = FactoryGirl.create :adminrole
  @testadmin = FactoryGirl.create :admin
  @testuserrole = FactoryGirl.create :userrole
  @testuser = FactoryGirl.create :user
  @testbanrole = FactoryGirl.create :banrole
  @testban = FactoryGirl.create :ban
end

And(/^I am signed in$/) do
  visit '/users/sign_in'
  fill_in "Username", :with => "testadmin"
  fill_in "Password", :with => "password"
  click_button 'Sign In'
  visit '/welcome/index'
end

When(/^I visit the main page$/) do
  visit '/welcome/index'
end
Then(/^I should see a my username at the top right page$/) do
  expect(page).to have_content "testadmin"
end
And(/^I should see an image for clicking to user management page$/) do
  expect(page).to have_css("img[src*='batch_batch_user.jpg']")
end
When(/^I click the link to user administration pages$/) do
  visit '/users/status'
end
Then(/^I should see user information$/) do
  expect(page).to have_content "testadmin"
  expect(page).to have_content "testuser"
  expect(page).to have_content "testban"
end
And(/^I should see their role$/) do
  #save_and_open_page
  expect(page).to have_content "admin"
  expect(page).to have_content "user"
  expect(page).to have_content "banned"
end




