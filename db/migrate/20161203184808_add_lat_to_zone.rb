class AddLatToZone < ActiveRecord::Migration
  def change
    add_column :zones, :Lat, :string
  end
end
