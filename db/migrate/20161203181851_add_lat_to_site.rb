class AddLatToSite < ActiveRecord::Migration
  def change
    add_column :sites, :Lat, :string
  end
end
