class CreateParkingtotals < ActiveRecord::Migration
  def change
    create_table :parkingtotals do |t|
      t.references :zone, index: true, foreign_key: true
      t.string :free

      t.timestamps null: false
    end
  end
end
